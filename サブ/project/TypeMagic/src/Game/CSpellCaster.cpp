#include "CSpellCaster.h"
#include "SpellDefine.h"
#include "CBall.h"
#include "CBolt.h"
#include "CBreath.h"

// コンストラクタ
CSpellCaster::CSpellCaster(CObjectBase* owner)
	: mpOwner(owner)
	, mpOpponent(nullptr)
	, mGenerateNum(0)
	, mSpellElemental(ESpellElementalType::eNeutral)
	, mSpellShape(ESpellShapeType::eBall)
	, mElapsedTime(0.0f)
	, mStep(0)
	, mIsSpellCasting(false)
	, mGenerateSpeed(0.0f)
	, mIdleTime(0.0f)
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
		// 時間経過
		mElapsedTime += Times::DeltaTime();
		// 生成速度の数値を超えたら
		if (mElapsedTime > mGenerateSpeed)
		{
			mElapsedTime = 0.0f;
			// 次へ
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
		if (mGenerateNum == BALL_MAX_GENERATE_NUM)
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
		// 時間経過
		mElapsedTime += Times::DeltaTime();
		// 待機時間を超えたら
		if (mElapsedTime > mIdleTime)
		{
			mElapsedTime = 0.0f;
			// 次へ
			mStep++;
		}
		break;

		// 順番に発射状態へ
	case 3:
		// 発射状態へ
		mSpells[mGenerateNum - 1]->ChangeState(CSpellBase::EState::eShooting);

		mGenerateNum--;
		// 全て発射状態にしたので
		if (mGenerateNum <= 0)
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
		mIsSpellCasting = false;
		break;
	}
	
}

// ボルト呪文の詠唱
void CSpellCaster::CastBolt()
{
}

// ブレス呪文の詠唱
void CSpellCaster::CastBreath()
{
}

// テレポート呪文の詠唱
void CSpellCaster::CastTeleport()
{
}

// テレポート呪文の詠唱
void CSpellCaster::CastShield()
{
}

// リフレクター呪文の詠唱
void CSpellCaster::CastReflector()
{
}

// 生成速度と待機時間の設定
void CSpellCaster::SetTime(ESpellElementalType elemental, ESpellShapeType shape)
{
	float generateSpeed = 0.0f;	// 生成速度
	float idleTime = 0.0f;		// 待機時間

	switch (elemental)
	{
	case ESpellElementalType::eFire:
		generateSpeed += FIRE_GENERATE_SPEED;
		idleTime += FIRE_IDLE_TIME;
		break;
	case ESpellElementalType::eWind:
		generateSpeed += WIND_GENERATE_SPEED;
		idleTime += WIND_IDLE_TIME;
		break;
	case ESpellElementalType::eEarth:
		generateSpeed += EARTH_GENERATE_SPEED;
		idleTime += EARTH_IDLE_TIME;
		break;
	case ESpellElementalType::eThunder:
		generateSpeed += THUNDER_GENERATE_SPEED;
		idleTime += THUNDER_IDLE_TIME;
		break;
	case ESpellElementalType::eWater:
		generateSpeed += WATER_GENERATE_SPEED;
		idleTime += WATER_IDLE_TIME;
		break;
	case ESpellElementalType::eNeutral:
		generateSpeed += NEUTRAL_GENERATE_SPEED;
		idleTime += NEUTRAL_IDLE_TIME;
		break;
	}

	switch (shape)
	{
	case ESpellShapeType::eBall:
		generateSpeed += BALL_GENERATE_SPEED;
		idleTime += BALL_IDLE_TIME;
		break;
	case ESpellShapeType::eBolt:
		generateSpeed += BOLT_GENERATE_SPEED;
		idleTime += BOLT_IDLE_TIME;
		break;
	case ESpellShapeType::eBreath:
		generateSpeed += BREATH_GENERATE_SPEED;
		idleTime += BREATH_IDLE_TIME;
		break;
	case ESpellShapeType::eTeleport:
		generateSpeed += TELEPORT_GENERATE_SPEED;
		idleTime += TELEPORT_IDLE_TIME;
		break;
	case ESpellShapeType::eShield:
		generateSpeed += SHIELD_GENERATE_SPEED;
		idleTime += SHIELD_IDLE_TIME;
		break;
	case ESpellShapeType::eReflector:
		generateSpeed += REFLECTOR_GENERATE_SPEED;
		idleTime += REFLECTOR_IDLE_TIME;
		break;
	}

	mGenerateSpeed = generateSpeed;
	mIdleTime = idleTime;
}

// 詠唱開始
bool CSpellCaster::CastStart(ESpellElementalType elemental
	,ESpellShapeType shape)
{
	// 詠唱中なので開始失敗
	if (mIsSpellCasting) return false;

	// 詠唱開始
	mIsSpellCasting = true;
	mSpellElemental = elemental;
	mSpellShape = shape;
	mElapsedTime = 0.0f;
	mStep = 0;
	mGenerateNum = 0;
	mSpells.clear();

	// 生成速度と待機時間を求めて設定する
	SetTime(elemental, shape);
}

// 対戦相手を設定
void CSpellCaster::SetOpponent(CObjectBase* opponent)
{
	mpOpponent = opponent;
}
