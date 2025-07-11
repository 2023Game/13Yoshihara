#include "CSpellCaster.h"
#include "SpellDefine.h"
#include "CBall.h"
#include "CBolt.h"
#include "CBreath.h"
#include "CTeleport.h"
#include "CShield.h"
#include "CReflector.h"
#include "CCharaStatusBase.h"

// コンストラクタ
CSpellCaster::CSpellCaster(CObjectBase* owner)
	: mpOwner(owner)
	, mpOpponent(nullptr)
	, mGenerateNum(0)
	, mSpellElemental(ESpellElementalType::eNeutral)
	, mSpellShape(ESpellShapeType::eError)
	, mElapsedTime(0.0f)
	, mStep(0)
	, mIsSpellCasting(false)
	, mGenerateInterval(0.0f)
	, mIdleTime(0.0f)
	, mIsShield(false)
{
}

// デストラクタ
CSpellCaster::~CSpellCaster()
{
}

// 更新
void CSpellCaster::Update()
{
	// 詠唱中でないなら処理しない
	if (!mIsSpellCasting) return;

	// 形
	switch (mSpellShape)
	{
	case ESpellShapeType::eBall:		CastBall();			break;
	case ESpellShapeType::eBolt:		CastBolt();			break;
	case ESpellShapeType::eBreath:		CastBreath();		break;
	case ESpellShapeType::eTeleport:	CastTeleport();		break;
	case ESpellShapeType::eShield:		CastShield();		break;
	case ESpellShapeType::eReflector:	CastReflector();	break;
	}
}

// ボール呪文の詠唱
void CSpellCaster::CastBall()
{
	switch (mStep)
	{
		// 生成間隔の待機
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// 呪文生成
	case 1:
	{
		// 生成座標のリスト
		CVector generatePosList[] = {
			BALL_OFFSET_POS_1,
			BALL_OFFSET_POS_2,
			BALL_OFFSET_POS_3,
			BALL_OFFSET_POS_4,
			BALL_OFFSET_POS_5,
		};
		// 生成座標
		CVector offsetPos = generatePosList[mGenerateNum];
		// 持ち主の方向で設定
		offsetPos =
			mpOwner->VectorX() * offsetPos.X() +
			mpOwner->VectorY() * offsetPos.Y() +
			mpOwner->VectorZ() * offsetPos.Z();

		// ボール呪文を生成
		CBall* ball = new CBall(mSpellElemental
			, mpOwner, mpOpponent);
		// 座標を設定
		ball->Position(mpOwner->Position() + offsetPos);
		// リストに追加
		mSpells.push_back(ball);
		// 生成数をプラス
		mGenerateNum++;

		// 最大数生成したら
		if (mGenerateNum >= BALL_MAX_GENERATE_NUM)
		{
			// 次へ
			mStep++;
		}
		// してないので
		else
		{
			// 戻る
			mStep--;
		}
		break;
	}
		// 発射待機
	case 2:
		if (WaitShoot())
		{
			mStep++;
		}
		break;

		// 順番に発射状態へ
	case 3:
		// 全て発射状態にしたので
		if (Shoot())
		{
			// 次へ
			mStep++;
		}
		// してないので
		else
		{
			// 戻る
			mStep--;
		}

		break;

	case 4:
		// 詠唱終了
		CastEnd();
		break;
	}
	
}

// ボルト呪文の詠唱
void CSpellCaster::CastBolt()
{
	switch (mStep)
	{
		// 生成間隔の待機
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// 呪文生成
	case 1:
	{
		// 生成座標
		CVector offsetPos = BOLT_OFFSET_POS;
		// 持ち主の方向で設定
		offsetPos =
			mpOwner->VectorX() * offsetPos.X() +
			mpOwner->VectorY() * offsetPos.Y() +
			mpOwner->VectorZ() * offsetPos.Z();

		// ボルト呪文を生成
		CBolt* bolt = new CBolt(mSpellElemental
			, mpOwner, mpOpponent);
		// 座標を設定
		bolt->Position(mpOwner->Position() + offsetPos);
		// リストに追加
		mSpells.push_back(bolt);
		// 生成数をプラス
		mGenerateNum++;

		// 次へ
		mStep++;

		break;
	}

		// 発射待機
	case 2:
		if (WaitShoot())
		{
			mStep++;
		}
		break;

		// 順番に発射状態へ
	case 3:
		// 全て発射状態にしたので
		if (Shoot())
		{
			// 次へ
			mStep++;
		}
		// してないので
		else
		{
			// 戻る
			mStep--;
		}

		break;

		// 詠唱終了
	case 4:
		CastEnd();
		break;
	}
}

// ブレス呪文の詠唱
void CSpellCaster::CastBreath()
{
	switch (mStep)
	{
		// 生成間隔の待機
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// 呪文生成
	case 1:
	{
		// 生成座標
		CVector offsetPos = BREATH_OFFSET_POS;
		// 持ち主の方向で設定
		offsetPos =
			mpOwner->VectorX() * offsetPos.X() +
			mpOwner->VectorY() * offsetPos.Y() +
			mpOwner->VectorZ() * offsetPos.Z();

		// ブレス呪文を生成
		CBreath* breath = new CBreath(mSpellElemental
			, mpOwner, mpOpponent);
		// 座標を設定
		breath->Position(mpOwner->Position() + offsetPos);
		// リストに追加
		mSpells.push_back(breath);
		// 生成数をプラス
		mGenerateNum++;

		// 最大数生成したら
		if (mGenerateNum >= BREATH_MAX_GENERATE_NUM)
		{
			// 次へ
			mStep++;
		}
		// まだなら
		else
		{
			// 前へ
			mStep--;
		}

		break;
	}

		// 詠唱終了
	case 2:
		CastEnd();
		break;
	}
}

// テレポート呪文の詠唱
void CSpellCaster::CastTeleport()
{
	switch (mStep)
	{
		// 生成間隔の待機
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// 呪文生成
	case 1:
	{
		// テレポート呪文を生成
		CTeleport* teleport = new CTeleport(mSpellElemental
			, mpOwner, mpOwner);
		// 座標を設定
		teleport->Position(mpOwner->Position());
		// リストに追加
		mSpells.push_back(teleport);
		// 次へ
		mStep++;
		break;
	}

		// 詠唱終了
	case 2:
		// 削除フラグが経っていたら
		if (mSpells[0]->IsKill())
		{
			CastEnd();
		}
		break;
	}
}

// シールド呪文の詠唱
void CSpellCaster::CastShield()
{
	switch (mStep)
	{
		// 生成間隔の待機
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// 呪文生成
	case 1:
	{
		// シールド呪文を生成
		CShield* shield = new CShield(mSpellElemental
			, mpOwner, mpOwner);
		// リストに追加
		mSpells.push_back(shield);
		// 生成数を増加
		mGenerateNum++;

		// 最大数生成したら
		if (mGenerateNum >= SHIELD_MAX_GENERATE_NUM)
		{
			// 次へ
			mStep++;
		}
		// してないなら
		else
		{
			// 前へ
			mStep--;
		}
		break;
	}

	// 詠唱終了
	case 2:
		CastEnd();
		break;
	}
}

// リフレクター呪文の詠唱
void CSpellCaster::CastReflector()
{
	switch (mStep)
	{
		// 生成間隔の待機
	case 0:
		if (WaitGenerate())
		{
			mStep++;
		}
		break;

		// 呪文生成
	case 1:
	{		
		// 生成座標
		CVector offsetPos = REFLECTOR_OFFSET_POS;
		// 持ち主の方向で設定
		offsetPos =
			mpOwner->VectorX() * offsetPos.X() +
			mpOwner->VectorY() * offsetPos.Y() +
			mpOwner->VectorZ() * offsetPos.Z();

		// リフレクター呪文を生成
		CReflector* reflector = new CReflector(mSpellElemental
			, mpOwner, mpOwner);
		// 座標設定
		reflector->Position(mpOwner->Position() + offsetPos);
		// リストに追加
		mSpells.push_back(reflector);
		// 生成数を増加
		mGenerateNum++;

		// 次へ
		mStep++;

		break;
	}

	// 詠唱終了
	case 2:
		CastEnd();
		break;
	}
}

// 生成速度と待機時間と消費MPの設定
void CSpellCaster::SetTimeAndMp(ESpellElementalType elemental, ESpellShapeType shape)
{
	float generateInterval = 0.0f;	// 生成間隔
	float idleTime = 0.0f;			// 待機時間
	int useMp = 0;					// 使用するMP

	switch (elemental)
	{
	case ESpellElementalType::eFire:
		generateInterval += FIRE_GENERATE_INTERVAL;
		idleTime += FIRE_IDLE_TIME;
		useMp += FIRE_USE_MP;
		break;
	case ESpellElementalType::eWind:
		generateInterval += WIND_GENERATE_INTERVAL;
		idleTime += WIND_IDLE_TIME;
		useMp += WIND_USE_MP;
		break;
	case ESpellElementalType::eEarth:
		generateInterval += EARTH_GENERATE_INTERVAL;
		idleTime += EARTH_IDLE_TIME;
		useMp += EARTH_USE_MP;
		break;
	case ESpellElementalType::eThunder:
		generateInterval += THUNDER_GENERATE_INTERVAL;
		idleTime += THUNDER_IDLE_TIME;
		useMp += THUNDER_USE_MP;
		break;
	case ESpellElementalType::eWater:
		generateInterval += WATER_GENERATE_INTERVAL;
		idleTime += WATER_IDLE_TIME;
		useMp += WATER_USE_MP;
		break;
	case ESpellElementalType::eNeutral:
		generateInterval += NEUTRAL_GENERATE_INTERVAL;
		idleTime += NEUTRAL_IDLE_TIME;
		useMp += NEUTRAL_USE_MP;
		break;
	}

	switch (shape)
	{
	case ESpellShapeType::eBall:
		generateInterval += BALL_GENERATE_INTERVAL;
		idleTime += BALL_IDLE_TIME;
		useMp += BALL_USE_MP;
		break;
	case ESpellShapeType::eBolt:
		generateInterval += BOLT_GENERATE_INTERVAL;
		idleTime += BOLT_IDLE_TIME;
		useMp += BOLT_USE_MP;
		break;
		// ブレスの場合ブレスの生成間隔を設定
	case ESpellShapeType::eBreath:
		generateInterval = BREATH_GENERATE_INTERVAL;
		idleTime += BREATH_IDLE_TIME;
		useMp += BREATH_USE_MP;
		break;
		// テレポートの場合テレポートの待機時間を設定
	case ESpellShapeType::eTeleport:
		generateInterval += TELEPORT_GENERATE_INTERVAL;
		idleTime = TELEPORT_IDLE_TIME;
		useMp += TELEPORT_USE_MP;
		break;
		// シールドの場合シールドの生成間隔を設定
	case ESpellShapeType::eShield:
		generateInterval = SHIELD_GENERATE_INTERVAL;
		idleTime += SHIELD_IDLE_TIME;
		useMp += SHIELD_USE_MP;
		break;
	case ESpellShapeType::eReflector:
		generateInterval += REFLECTOR_GENERATE_INTERVAL;
		idleTime += REFLECTOR_IDLE_TIME;
		useMp += REFLECTOR_USE_MP;
		break;
	}

	mGenerateInterval = generateInterval;
	mIdleTime = idleTime;
	mUseMp = useMp;
}

// 生成待機
bool CSpellCaster::WaitGenerate()
{		
	// 時間経過
	mElapsedTime += Times::DeltaTime();
	// 生成間隔が過ぎたら
	if (mElapsedTime > mGenerateInterval)
	{
		mElapsedTime = 0.0f;
		// 待機終了
		return true;
	}

	// 待機中
	return false;
}

// 発射待機
bool CSpellCaster::WaitShoot()
{		
	// 時間経過
	mElapsedTime += Times::DeltaTime();
	// 待機時間を超えたら
	if (mElapsedTime > mIdleTime)
	{
		mElapsedTime = 0.0f;
		// 待機終了
		return true;
	}

	// 待機中
	return false;
}

// 発射
bool CSpellCaster::Shoot()
{
	// 発射状態へ
	mSpells[mGenerateNum - 1]->ChangeState(CSpellBase::EState::eShooting);

	mGenerateNum--;
	// 全て発射状態にした
	if (mGenerateNum <= 0)
	{
		// 発射終了
		return true;
	}

	// まだ残っている
	return false;
}

// 詠唱開始
bool CSpellCaster::CastStart(ESpellElementalType elemental
	,ESpellShapeType shape)
{
	// 詠唱中なので開始失敗
	if (mIsSpellCasting) return false;

	// 生成速度と待機時間と消費MPを求めて設定する
	SetTimeAndMp(elemental, shape);

	// 持ち主のステータスを取得
	CCharaStatusBase* charaStatus = dynamic_cast<CCharaStatusBase*>(mpOwner);
	// Mpを消費できなければ開始失敗
	if (!charaStatus->UseMp(mUseMp)) return false;

	// 詠唱開始
	mIsSpellCasting = true;
	mSpellElemental = elemental;
	mSpellShape = shape;
	mElapsedTime = 0.0f;
	mStep = 0;
	mGenerateNum = 0;
	mSpells.clear();
}

// 詠唱終了
void CSpellCaster::CastEnd()
{
	// 詠唱終了
	mIsSpellCasting = false;
	mSpellElemental = ESpellElementalType::eError;
	mSpellShape = ESpellShapeType::eError;
	mElapsedTime = 0.0f;
	mStep = 0;
	mGenerateNum = 0;
	mSpells.clear();
}

// シールドを貼っているかを設定
void CSpellCaster::SetIsShield(bool enable)
{
	mIsShield = enable;
}

// 対戦相手を設定
void CSpellCaster::SetOpponent(CObjectBase* opponent)
{
	mpOpponent = opponent;
}

// 対戦相手を取得
CObjectBase* CSpellCaster::GetOpponent() const
{
	return mpOpponent;
}

// 詠唱中かを取得
bool CSpellCaster::IsCasting() const
{
	return mIsSpellCasting;
}
