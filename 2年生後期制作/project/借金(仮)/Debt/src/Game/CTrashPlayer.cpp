#include "CTrashPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CTrashEnemy.h"
#include "CGaugeUI2D.h"
#include "CTrashBag.h"
#include "CTrashField.h"

// 衝突相手のクラスを取得するためのインクルード
#include "CVehicleBase.h"
#include "CCollector.h"

// コライダのインクルード
#include "CColliderCapsule.h"
#include "CColliderSphere.h"

// アニメーションのパス
#define ANIM_PATH "Character\\TrashBox\\anim\\"
/*
 プレイヤーのアニメーションデータのテーブル
 (と)＝蓋が閉じている状態からのアニメーション
 (開)＝蓋が開いている状態からのアニメーション
 (開閉)＝両方の状態兼用のアニメーション
*/
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",								true,	0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"Idle_Close.x",			true,	30.0f,	1.0f},	// 待機					（と）
	{ ANIM_PATH"Idle_Open.x",			true,	30.0f,	1.0f},	// 待機					（開）
	{ ANIM_PATH"Move_Close.x",			true,	80.0f,	1.0f},	// 移動					（と）
	{ ANIM_PATH"Move_Open.x",			true,	80.0f,	1.0f},	// 移動					（開）
	{ ANIM_PATH"Damage_Close_Start.x",	false,	10.0f,	1.0f},	// 被弾開始				（と）
	{ ANIM_PATH"Damage_Open_Start.x",	false,	10.0f,	1.0f},	// 被弾開始				（開）
	{ ANIM_PATH"Damage.x",				true,	11.0f,	1.0f},	// 被弾ノックバック		（開閉）
	{ ANIM_PATH"Damage_End.x",			false,	51.0f,	1.0f},	// 被弾終了				（開閉）
	{ ANIM_PATH"Jump_Close_Start.x",	false,	20.0f,	1.0f},	// ジャンプ開始			（と）
	{ ANIM_PATH"Jump_Close.x",			true,	11.0f,	1.0f},	// ジャンプ中			（と）
	{ ANIM_PATH"Jump_Close_End.x",		false,	21.0f,	1.0f},	// ジャンプ終了			（と）
	{ ANIM_PATH"Jump_Open_Start.x",		false,	20.0f,	1.0f},	// ジャンプ開始			（開）
	{ ANIM_PATH"Jump_Open.x",			true,	11.0f,	1.0f},	// ジャンプ中			（開）
	{ ANIM_PATH"Jump_Open_End.x",		false,	21.0f,	1.0f},	// ジャンプ終了			（開）
	{ ANIM_PATH"Attack_Start.x",		false,	35.0f,	1.0f},	// 攻撃開始				（開閉）
	{ ANIM_PATH"Attack.x",				false,	11.0f,	1.0f},	// 攻撃中				（開閉）
	{ ANIM_PATH"Attack_End.x",			false,	69.0f,	1.0f},	// 攻撃終了				（開閉）
	{ ANIM_PATH"Critical_Start.x",		false,	35.0f,	1.0f},	// クリティカル攻撃開始	（開閉）
	{ ANIM_PATH"Critical.x",			false,	11.0f,	1.0f},	// クリティカル攻撃中	（開閉）
	{ ANIM_PATH"Critical_End.x",		false,	69.0f,	1.0f},	// クリティカル攻撃終了	（開閉）
	{ ANIM_PATH"Open.x",				false,	10.0f,	1.0f},	// 蓋を開く				（と）
	{ ANIM_PATH"Close.x",				false,	10.0f,	1.0f},	// 蓋を閉じる			（開）
	{ ANIM_PATH"Death.x",				false,   5.0f,  1.0f},	// 死亡					（開）
};

#define BODY_RADIUS 2.5f	// 本体のコライダ―の半径
#define BODY_HEIGHT 25.0f	// 本体のコライダ―の高さ
#define BODY_WIDTH 50.0f	// 本体のコライダ―の幅

// 攻撃コライダ―
#define ATTACK_COL_RADIUS	2.0f	// 半径
#define ATTACK_COL_HEIGHT	25.0f	// 高さ
#define ATTACK_COL_WIDTH	70.0f	// 幅
// オフセット座標
#define ATTACK_COL_OFFSET_POS CVector(0.0f,0.0f,100.0f)

// クリティカル攻撃コライダー
#define CRITICAL_COL_RADIUS		3.0f		// 半径
#define CRITICAL_COL_HEIGHT		25.0f		// 高さ
#define CRITICAL_COL_WIDTH		140.0f		// 幅
// オフセット座標
#define CRITICAL_COL_OFFSET_POS CVector(0.0f,0.0f,160.0f)

// モーションブラーを掛ける時間
#define MOTION_BLUR_TIME 3.0f
// モーションブラーの幅
#define MOTION_BLUR_WIDTH 1.0f
// モーションブラーの反復回数
#define MOTION_BLUR_COUNT 5

#define SCALE 0.1f	// スケール

// Hpゲージの画像のパス
#define HP_GAUGE_PATH "UI\\trashbox_player_hp_gauge.png"
#define HP_GAUGE_SIZE 1.5f	// Hpゲージの大きさの倍率
#define HP_GAUGE_OFFSET_POS CVector(10.0f,10.0f,0.0f)

// ゴミ袋を落とすオフセット座標
#define TRASH_BAG_OFFSET_POS CVector(0.0f,5.0f,0.0f)

// コンストラクタ
CTrashPlayer::CTrashPlayer()
	: CPlayerBase()
	, CTrashPlayerStatus()
	, mState(EState::eIdle)
	, mIsOpen(false)
	, mIsJump(false)
	, mIsStickCollector(false)
	, mpStickCollector(nullptr)
{
	// Hpゲージを設定
	mpHpGauge = new CGaugeUI2D(this, HP_GAUGE_PATH);
	mpHpGauge->Position(HP_GAUGE_OFFSET_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());
	mpHpGauge->Size(HP_GAUGE_SIZE);
	// 大きさの調整
	Scale(SCALE, SCALE, SCALE);
	// アニメーションとモデルの初期化
	InitAnimationModel("TrashPlayer", &ANIM_DATA);

	// 本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector( BODY_WIDTH - BODY_RADIUS / SCALE, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS / SCALE, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	// 地形、敵、回収員、攻撃、車両、キャラ探知用、ゴミ袋
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eEnemy, ETag::eVehicle,ETag::eTrashBag});
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,
		ELayer::eEnemy, ELayer::eCollector, ELayer::eAttackCol, ELayer::eVehicle,
		ELayer::eCharaSearch,ELayer::eTrashBag});

	// 攻撃コライダー
	mpAttackCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, ATTACK_COL_HEIGHT, ATTACK_COL_WIDTH - ATTACK_COL_RADIUS * 10),
		CVector(0.0f, ATTACK_COL_HEIGHT, -ATTACK_COL_WIDTH + ATTACK_COL_RADIUS * 10),
		ATTACK_COL_RADIUS
	);
	// クリティカル攻撃コライダー
	mpCriticalCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, CRITICAL_COL_HEIGHT, CRITICAL_COL_WIDTH - CRITICAL_COL_RADIUS * 10),
		CVector(0.0f, CRITICAL_COL_HEIGHT, -CRITICAL_COL_WIDTH + CRITICAL_COL_RADIUS * 10),
		CRITICAL_COL_RADIUS
	);

	// 敵と回収員と車両と衝突判定する
	mpAttackCol->SetCollisionTags({ ETag::eEnemy,ETag::eVehicle });
	mpAttackCol->SetCollisionLayers({ ELayer::eEnemy,ELayer::eCollector,ELayer::eVehicle });
	mpCriticalCol->SetCollisionTags({ ETag::eEnemy,ETag::eVehicle });
	mpCriticalCol->SetCollisionLayers({ ELayer::eEnemy,ELayer::eCollector,ELayer::eVehicle });
	
	// 自分の前に位置調整
	mpAttackCol->Position(ATTACK_COL_OFFSET_POS);
	mpCriticalCol->Position(CRITICAL_COL_OFFSET_POS);

	// 攻撃コライダーは最初はオフにしておく
	mpAttackCol->SetEnable(false);
	mpCriticalCol->SetEnable(false);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle_Close);
}

// デストラクタ
CTrashPlayer::~CTrashPlayer()
{
	SAFE_DELETE(mpCriticalCol);
}

// 更新
void CTrashPlayer::Update()
{
	// 現在の状態に合わせて更新処理を切り替え
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::eDamageStart:		UpdateDamageStart();	break;
	case EState::eDamage:			UpdateDamage();			break;
	case EState::eDamageEnd:		UpdateDamageEnd();		break;
	case EState::eJumpStart:		UpdateJumpStart();		break;
	case EState::eJump:				UpdateJump();			break;
	case EState::eJumpEnd:			UpdateJumpEnd();		break;
	case EState::eAttackStart:		UpdateAttackStart();	break;
	case EState::eAttack:			UpdateAttack();			break;
	case EState::eAttackEnd:		UpdateAttackEnd();		break;
	case EState::eCriticalStart:	UpdateCriticalStart();	break;
	case EState::eCritical:			UpdateCritical();		break;
	case EState::eCriticalEnd:		UpdateCriticalEnd();	break;
	case EState::eOpenClose:		UpdateOpenClose();		break;
	case EState::eDeath:			UpdateDeath();			break;
	}

	// 待機中とジャンプ中と
	// 攻撃開始終了、クリティカル開始終了は、移動処理を行う
	if (mState == EState::eIdle ||
		mIsJump ||
		mState == EState::eAttackStart ||
		mState == EState::eAttackEnd||
		mState==EState::eCriticalStart||
		mState == EState::eCriticalEnd)
	{
		UpdateMove();
	}

	// キャラクターの更新
	CPlayerBase::Update();

	// HPゲージを更新
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());

#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("PlayerIsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("PlayerIsJump:%s\n", mIsJump ? "true" : "false");
	CDebugPrint::Print("PlayerHp:%d\n", GetHp());
	CDebugPrint::Print("PlayerIsCollector:%s\n", mIsStickCollector ? "true" : "false");
	CDebugPrint::Print("PlayerTrashBagNum:%d\n", GetTrashBag());
	CDebugPrint::Print("PlayerGoldBagNum:%d\n", GetGoldTrashBag());
#endif
}

// 衝突処理
void CTrashPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CPlayerBase::Collision(self, other, hit);

	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// 衝突した相手が車両なら
		if (other->Layer() == ELayer::eVehicle)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
		// 衝突した相手が敵なら
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 押し戻しベクトル
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
		// 衝突した相手が回収員なら
		else if (other->Layer() == ELayer::eCollector)
		{
			// 回収員のクラスを取得
			CCollector* collector = dynamic_cast<CCollector*>(other->Owner());

			// 攻撃中でないなら
			if (!collector->IsAttacking())
			{
				// 押し戻しベクトル
				CVector adjust = hit.adjust;
				adjust.Y(0.0f);

				// 押し戻しベクトルの分、座標を移動
				Position(Position() + adjust * hit.weight);
			}
			// 攻撃中なら
			else if (collector->IsAttacking())
			{
				// 蓋が開いていない場合
				if (!mIsOpen)
				{
					// 蓋を開く
					ChangeState(EState::eOpenClose);
				}
			}
		}
	}
	// 攻撃コライダー
	else if (self == mpAttackCol)
	{
		// 衝突した相手が敵なら
		if (other->Layer() == ELayer::eEnemy)
		{
			// 敵クラスを取得
			CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());

			if (enemy != nullptr &&
				!IsAttackHitObj(enemy))
			{
				AddAttackHitObj(enemy);
				// 自分から相手の方向
				CVector direction = enemy->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// 相手が受けるノックバック速度に、
				// 自分が与えるノックバック速度を自分から相手の方向に設定
				enemy->SetKnockbackReceived(direction * GetKnockbackDealt());
				// 攻撃力分のダメージを与える
				enemy->TakeDamage(GetAttackPower(), this, GetPower());
			}
		}
		// 衝突した相手が回収員なら
		else if (other->Layer() == ELayer::eCollector)
		{
			// 回収員クラスを取得
			CCollector* collector = dynamic_cast<CCollector*>(other->Owner());

			if (collector != nullptr &&
				!IsAttackHitObj(collector))
			{
				AddAttackHitObj(collector);
				// 攻撃力分のダメージを与える
				collector->TakeDamage(GetAttackPower(), this);
			}
		}
		// 衝突した相手が車両なら
		else if (other->Layer() == ELayer::eVehicle)
		{
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			if (vehicle != nullptr &&
				!IsAttackHitObj(vehicle))
			{
				AddAttackHitObj(vehicle);
				// 攻撃力分のダメージを与える
				vehicle->TakeDamage(GetAttackPower(), this);
			}
		}
	}
	// クリティカル攻撃コライダ―
	else if (self == mpCriticalCol)
	{
		// 衝突した相手が敵なら
		if (other->Layer() == ELayer::eEnemy)
		{
			// 敵クラスを取得
			CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());

			if (enemy != nullptr &&
				!IsAttackHitObj(enemy))
			{
				AddAttackHitObj(enemy);
				// 自分から相手の方向
				CVector direction = enemy->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// 相手が受けるノックバック速度に、
				// 自分が与えるノックバック速度の2倍を自分から相手の方向に設定
				enemy->SetKnockbackReceived(direction * GetKnockbackDealt() * 2.0f);
				// クリティカルダメージを与える
				enemy->TakeCritical(GetAttackPower(), this, GetPower());
			}
		}
		// 衝突した相手が回収員なら
		else if (other->Layer() == ELayer::eCollector)
		{
			// 回収員クラスを取得
			CCollector* collector = dynamic_cast<CCollector*>(other->Owner());

			if (collector != nullptr &&
				!IsAttackHitObj(collector))
			{
				AddAttackHitObj(collector);
				// 攻撃力分のダメージを与える
				collector->TakeDamage(GetAttackPower(), this);
			}
		}
		// 衝突した相手が車両なら
		else if (other->Layer() == ELayer::eVehicle)
		{
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			if (vehicle != nullptr &&
				!IsAttackHitObj(vehicle))
			{
				AddAttackHitObj(vehicle);
				// 攻撃力分のダメージを与える
				vehicle->TakeCritical(GetAttackPower(), this);
			}
		}
	}
}

// 回収員がついているかを取得
bool CTrashPlayer::GetStickCollector() const
{
	return mIsStickCollector;
}

// 回収員がついているかを設定
void CTrashPlayer::SetStickCollector(bool stickCollector)
{
	mIsStickCollector = stickCollector;
}

// ついている回収員のポインタを取得
CCollector* CTrashPlayer::GetStickCollectorPointer() const
{
	return mpStickCollector;
}

// ついている回収員のポインタを設定
void CTrashPlayer::SetStickCollectorPointer(CCollector* collector)
{
	mpStickCollector = collector;
}

// 開いているかを取得
bool CTrashPlayer::GetOpen() const
{
	return mIsOpen;
}

// X軸の範囲外にいるかどうか
bool CTrashPlayer::AreaOutX()
{
	// 車道のXの範囲外にいるなら
	if (Position().X() >= ROAD_X_AREA ||
		Position().X() <= -ROAD_X_AREA)
	{
		return true;
	}
	// そうでないなら範囲内
	return false;
}

// Z軸の範囲外にいるかどうか
bool CTrashPlayer::AreaOutZ()
{
	// 車道のZの範囲外にいるなら
	if (Position().Z() >= ROAD_Z_AREA ||
		Position().Z() <= -ROAD_Z_AREA)
	{
		return true;
	}
	// そうでないなら範囲内
	return false;
}

// ゴミ袋を落とす処理
void CTrashPlayer::DropTrashBag(int power)
{
	// 落とす力が0以下なら処理しない
	if (power <= 0) return;

	// ゴミ袋を一つでも所持していたら落とす
	if (GetTrashBag() > 0)
	{
		// パワーの最終的な結果
		int powerResult = power;
		// ゴミ袋の数がパワーより少ない場合
		if (GetTrashBag() < power)
		{
			// パワーの最終的な結果をゴミ袋の数に設定
			powerResult = GetTrashBag();
		}
		// ゴミ袋の数を最終的なパワー分減らす
		SetTrashBag(-powerResult);
		for (int i = 0; i < powerResult; i++)
		{
			CTrashBag* trashBag = new CTrashBag(false);
			trashBag->Position(Position() + TRASH_BAG_OFFSET_POS * (i + 1));
			trashBag->SetThrowSpeed(VectorZ() * GetKnockbackDealt(), GetKnockbackDealt());
		}
	}
	// 通常のゴミ袋を一つも持っていない場合かつ
	// ゴールドゴミ袋持っている場合に落とす
	else if (GetGoldTrashBag() > 0)
	{
		// パワーの最終的な結果
		int powerResult = power;
		// ゴミ袋の数がパワーより少ない場合
		if (GetGoldTrashBag() < power)
		{
			// パワーの最終的な結果をゴミ袋の数に設定
			powerResult = GetGoldTrashBag();
		}
		// ゴミ袋の数を最終的なパワー分減らす
		SetGoldTrashBag(-powerResult);
		for (int i = 0; i < powerResult; i++)
		{
			CTrashBag* trashBag = new CTrashBag(true);
			trashBag->Position(Position() + TRASH_BAG_OFFSET_POS * (i + 1));
			trashBag->SetThrowSpeed(VectorZ() * GetKnockbackDealt(), GetKnockbackDealt());
		}
	}
}

/*
アクションのキー入力
回収員がついていないときのみ入力可能
*/
void CTrashPlayer::ActionInput()
{
	// 回収員がくっついているかつ
	if (GetStickCollector())
	{
		// 回収員のポインタ
		CCollector* collector = GetStickCollectorPointer();
		// ポインタがnullでないかつ、
		// 回収員がお仕置き用の場合キー入力不可
		if (collector != nullptr &&
			GetStickCollectorPointer()->GetPunisher())
		{
			return;
		}
	}

	// スペースでジャンプ
	if (CInput::PushKey(VK_SPACE))
	{
		if (mState == EState::eOpenClose)
		{
			// ジャンプ速度の設定
			mMoveSpeedY = GetJumpSpeed();
			mIsGrounded = false;
			mIsJump = true;
			ChangeState(EState::eJump);
		}
		else
		{
			ChangeState(EState::eJumpStart);
		}
	}
	// 左クリックで攻撃
	if (CInput::PushKey(VK_LBUTTON))
	{
		// 1から100までの100個の数から乱数を取得
		int random = Math::Rand(1, 100);
		// クリティカル確率以下の値ならクリティカル攻撃
		if (random <= GetCriticalChance())
		{
			ChangeState(EState::eCriticalStart);
		}
		// それ以外の時は通常攻撃
		else
		{
			ChangeState(EState::eAttackStart);
		}
	}
	// 回収員がついていないときのみキー入力できる
	if (!mIsStickCollector)
	{
		// 右クリックで蓋の開閉
		if (CInput::PushKey(VK_RBUTTON))
		{
			ChangeState(EState::eOpenClose);
		}
	}
}

// 待機状態
void CTrashPlayer::UpdateIdle()
{
	if (mIsGrounded)
	{
		// キー入力可能
		ActionInput();
	}
}

// 移動
void CTrashPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * GetBaseMoveSpeed();

		// ジャンプ中か攻撃開始終了、
		// クリティカル開始終了の状態でなければ、
		// 移動アニメーションに切り替え
		if (!mIsJump &&
			mState != EState::eAttackStart &&
			mState != EState::eAttackEnd&&
			mState!=EState::eCriticalStart&&
			mState != EState::eCriticalEnd)
		{
			ChangeState(EState::eIdle);
			if (!mIsOpen)
				ChangeAnimation((int)EAnimType::eMove_Close);
			else
				ChangeAnimation((int)EAnimType::eMove_Open);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			if (!mIsOpen)
				ChangeAnimation((int)EAnimType::eIdle_Close);
			else
				ChangeAnimation((int)EAnimType::eIdle_Open);
		}
	}
}

// 被弾開始
void CTrashPlayer::UpdateDamageStart()
{
	switch (mStateStep)
	{
	case 0:
		// ジャンプ中に攻撃されたときのためfalse
		mIsJump = false;
		mIsDamage = true;
		mMoveSpeed = CVector::zero;
		// 閉じていたら
		if (!mIsOpen)
		{
			// 閉じた被弾開始アニメーション再生
			ChangeAnimation((int)EAnimType::eDamage_Close_Start);
		}
		// 開いていたら
		else
		{
			// 開いた被弾開始アニメーション再生
			ChangeAnimation((int)EAnimType::eDamage_Open_Start);
		}
		mStateStep++;
		break;

	case 1:
		// ノックバック時の飛び上がりの速度を
		// 受けるノックバック速度の半分に設定
		mMoveSpeedY = GetKnockbackReceived().Length() * 0.5f;
		// 受けるノックバック速度を移動に設定
		mMoveSpeed = GetKnockbackReceived();
		mIsGrounded = false;

		// 被弾状態へ
		ChangeState(EState::eDamage);
		break;
	}
}

// 被弾ノックバック
void CTrashPlayer::UpdateDamage()
{
	switch(mStateStep)
	{
	case 0:
		// 開いている
		mIsOpen = true;
		// 被弾アニメーション再生
		ChangeAnimation((int)EAnimType::eDamage);
		mStateStep++;
		break;

	case 1:
		// 地面についたら
		if (mIsGrounded)
		{
			// 被弾終了状態へ
			ChangeState(EState::eDamageEnd);
		}
		break;
	}
}

// 被弾終了
void CTrashPlayer::UpdateDamageEnd()
{
	switch (mStateStep)
	{
	case 0:
		// 受けるノックバックをリセット
		SetKnockbackReceived(CVector::zero);
		// 移動をゼロ
		mMoveSpeed = CVector::zero;
		// 被弾終了アニメーション再生
		ChangeAnimation((int)EAnimType::eDamage_End);
		mStateStep++;
		break;

	case 1:
		// アニメーションが60%進行したら
		if (GetAnimationFrameRatio() >= 0.60f)
		{
			mIsDamage = false;
			mStateStep++;
		}
		break;

	case 2:
		// 入力可能
		ActionInput();
		// 移動可能
		UpdateMove();
		// アニメーションが終了したら待機へ
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// ジャンプ開始
void CTrashPlayer::UpdateJumpStart()
{
	switch (mStateStep)
	{
	case 0:
		// ジャンプ開始時は移動をゼロ
		mMoveSpeed = CVector::zero;
		// 閉じていたら
		if (!mIsOpen)
		{
			// 閉じているジャンプアニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Close_Start);
		}
		// 開いていたら
		else
		{
			// 開いているジャンプアニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Open_Start);
		}
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// ジャンプ速度の設定
			mMoveSpeedY = GetJumpSpeed();
			mIsGrounded = false;
			mIsJump = true;
			// ジャンプ中状態へ
			ChangeState(EState::eJump);
		}
		break;
	}
}

// ジャンプ中
void CTrashPlayer::UpdateJump()
{
	switch (mStateStep)
	{
	case 0:
		// 閉じていたら
		if (!mIsOpen)
		{
			// 閉じているジャンプ中アニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Close);
		}
		// 開いていたら
		else
		{
			// 開いているジャンプ中アニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Open);
		}
		mStateStep++;
		break;

	case 1:
		// ジャンプ中は移動しながら開閉可能
		// 右クリックで蓋の開閉
		if (CInput::PushKey(VK_RBUTTON))
		{
			// 開閉状態へ
			ChangeState(EState::eOpenClose);
		}
		// 地面に付いたら
		if (mIsGrounded)
		{
			// ジャンプ終了状態へ
			ChangeState(EState::eJumpEnd);
		}
		break;
	}
}

// ジャンプ終了
void CTrashPlayer::UpdateJumpEnd()
{
	switch (mStateStep)
	{
	case 0:
		mIsJump = false;
		// 移動をゼロ
		mMoveSpeed = CVector::zero;
		// 閉じていたら
		if (!mIsOpen)
		{
			// 閉じているジャンプ終了アニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Close_End);
		}
		// 開いていたら
		else
		{
			// 開いているジャンプ終了アニメーション再生
			ChangeAnimation((int)EAnimType::eJump_Open_End);
		}
		mStateStep++;
		break;

	case 1:
		// アニメーションが50%以上進行したらモーションキャンセル可能
		if (GetAnimationFrameRatio() >= 0.50f)
		{
			// 入力可能
			ActionInput();
			// 移動可能
			UpdateMove();
		}
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 待機状態へ
			ChangeState(EState::eIdle);
		}
		break;
	}
}


// 攻撃開始
void CTrashPlayer::UpdateAttackStart()
{
	switch (mStateStep)
	{
	case 0:
		// 閉じているとき
		if (!mIsOpen)
		{
			// 開くアニメーション再生
			ChangeAnimation((int)EAnimType::eOpen);
			// 次のステップへ
			mStateStep++;
		}
		// 開いているとき
		else
		{
			// 攻撃開始アニメーション再生
			ChangeAnimation((int)EAnimType::eAttack_Start);
			// ステップ2へ
			mStateStep = 2;
		}
		break;

	case 1:
		// 開くアニメーションが終了したら
		if (IsAnimationFinished())
		{
			mIsOpen = true;
			// 攻撃開始アニメーションを再生
			ChangeAnimation((int)EAnimType::eAttack_Start);
			mStateStep++;
		}
		break;

	case 2:
		// 攻撃開始アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃中へ
			ChangeState(EState::eAttack);
			mMoveSpeed = CVector::zero;
		}
		break;
	}
}

// 攻撃中
void CTrashPlayer::UpdateAttack()
{
	switch (mStateStep)
	{
	case 0:
		// 攻撃アニメーション再生
		ChangeAnimation((int)EAnimType::eAttack);
		AttackStart();
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 攻撃終了へ
			ChangeState(EState::eAttackEnd);
			AttackEnd();
		}
		break;
	}
}

// 攻撃終了
void CTrashPlayer::UpdateAttackEnd()
{
	switch (mStateStep)
	{
	case 0:
		mIsOpen = true;
		// 攻撃終了アニメーション再生
		ChangeAnimation((int)EAnimType::eAttack_End);
		mStateStep++;
		break;

	case 1:
		// アニメーションが52%以上進行したらモーションキャンセル可能
		if (GetAnimationFrameRatio() >= 0.52f)
		{
			// 入力可能
			ActionInput();
			// 移動可能
			UpdateMove();
		}
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 待機状態へ
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// クリティカル開始
void CTrashPlayer::UpdateCriticalStart()
{
	switch (mStateStep)
	{
	case 0:
		// 閉じているとき
		if (!mIsOpen)
		{
			// 開くアニメーション再生
			ChangeAnimation((int)EAnimType::eOpen);
			// 次のステップへ
			mStateStep++;
		}
		// 開いているとき
		else
		{
			// クリティカル開始アニメーション再生
			ChangeAnimation((int)EAnimType::eCritical_Start);
			// ステップ2へ
			mStateStep = 2;
		}
		break;

	case 1:
		// 開くアニメーションが終了したら
		if (IsAnimationFinished())
		{
			mIsOpen = true;
			// クリティカル開始アニメーションを再生
			ChangeAnimation((int)EAnimType::eCritical_Start);
			mStateStep++;
		}
		break;

	case 2:
		// クリティカル開始アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// クリティカル中へ
			ChangeState(EState::eCritical);
			mMoveSpeed = CVector::zero;
		}
		break;
	}
}

// クリティカル中
void CTrashPlayer::UpdateCritical()
{
	switch (mStateStep)
	{
	case 0:
		// クリティカルアニメーション再生
		ChangeAnimation((int)EAnimType::eCritical);
		AttackStart();
		mStateStep++;
		break;

	case 1:
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// クリティカル終了へ
			ChangeState(EState::eCriticalEnd);
		}
		break;
	}
}

// クリティカル終了
void CTrashPlayer::UpdateCriticalEnd()
{
	switch (mStateStep)
	{
	case 0:
		// クリティカル終了アニメーション再生
		ChangeAnimation((int)EAnimType::eCritical_End);
		AttackEnd();
		mStateStep++;
		break;

	case 1:
		// アニメーションが52%以上進行したらモーションキャンセル可能
		if (GetAnimationFrameRatio() >= 0.52f)
		{
			// 入力可能
			ActionInput();
			// 移動可能
			UpdateMove();
		}
		// アニメーションが終了したら待機へ
		if (IsAnimationFinished())
		{
			// 待機状態へ
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// 蓋を開閉する
void CTrashPlayer::UpdateOpenClose()
{
	switch (mStateStep)
	{
	case 0:
		// ジャンプしていなければ
		if (!mIsJump)
		{
			// 移動をゼロ
			mMoveSpeed = CVector::zero;
		}

		// 閉じていたら
		if (!mIsOpen)
		{
			// 開くアニメーション再生
			ChangeAnimation((int)EAnimType::eOpen);
		}
		// 開いていたら
		else
		{
			// 閉じるアニメーション再生
			ChangeAnimation((int)EAnimType::eClose);
		}
		mStateStep++;
		break;

	case 1:
		// アニメーションが50%以上進行したら
		if (GetAnimationFrameRatio() >= 0.50f)
		{
			// 開き状態を変更
			mIsOpen = !mIsOpen;
			mStateStep++;
		}
		break;

	case 2:
		// ジャンプしていなければ
		if (!mIsJump)
		{
			// 入力可能
			ActionInput();
			// 移動可能
			UpdateMove();
		}
		// アニメーションが終了したら
		if (IsAnimationFinished())
		{
			// ジャンプしていないなら
			if (!mIsJump)
			{
				// 待機状態へ
				ChangeState(EState::eIdle);
			}
			// ジャンプしているならジャンプへ戻る
			else
			{
				// ジャンプ状態へ
				ChangeState(EState::eJump);
			}
		}
		break;
	}
}

// 死亡の更新処理
void CTrashPlayer::UpdateDeath()
{
	switch (mStateStep)
	{
		// ステップ0：死亡アニメーションを再生
	case 0:
		ChangeAnimation((int)EAnimType::eDeath);
		mMoveSpeed = CVector::zero;
		mStateStep++;
		break;

		// ステップ1：アニメーションが終了したら次のステップへ
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
		// ステップ1：TODO：ゲーム終了画面を表示する
	case 2:

		break;
	}
}

// 死亡処理
void CTrashPlayer::Death()
{
	// 死亡状態へ
	ChangeState(EState::eDeath);
}


// 攻撃中か
bool CTrashPlayer::IsAttacking() const
{
	// 攻撃中
	if (mState == EState::eAttack) return true;
	// クリティカル攻撃中
	if (mState == EState::eCritical) return true;

	// 攻撃中でない
	return false;
}

// 攻撃開始
void CTrashPlayer::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CXCharacter::AttackStart();

	// 攻撃中なら、攻撃コライダーをオン
	if (mState == EState::eAttack)
	{
		mpAttackCol->SetEnable(true);
	}
	// クリティカル攻撃中なら、クリティカルコライダ―をオン
	else if (mState == EState::eCritical)
	{
		mpCriticalCol->SetEnable(true);
	}
}

// 攻撃終了
void CTrashPlayer::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();

	// 攻撃コライダーをオフ
	mpAttackCol->SetEnable(false);
	mpCriticalCol->SetEnable(false);
}

// ダメージを受ける
void CTrashPlayer::TakeDamage(int damage, CObjectBase* causer, int dropNum)
{

	// 開いていればダメージを受ける
	if (mIsOpen)
	{
		// ダメージを受ける
		CCharaStatusBase::TakeDamage(damage, causer);
		// ゴミ袋を落とす
		DropTrashBag(dropNum);

		// 死亡していなければ
		if (!IsDeath())
		{
			// 攻撃を加えた相手の方向へ向く
			CVector targetPos = causer->Position();
			CVector vec = targetPos - Position();
			vec.Y(0.0f);
			Rotation(CQuaternion::LookRotation(vec.Normalized()));

			// 被弾開始状態へ移行
			ChangeState(EState::eDamageStart);
		}
	}
	// 閉じていればノーダメージで蓋が開くのみ
	else
	{
		// 開閉状態へ移行
		ChangeState(EState::eOpenClose);
	}
}

// クリティカルダメージを受ける
void CTrashPlayer::TakeCritical(int damage, CObjectBase* causer, int dropNum)
{
	// 開いていれば2倍のダメージを受ける
	if (mIsOpen)
	{
		// 攻撃力の2倍のダメージ
		int criticalDamage = damage * 2;
		// 落とす数の2倍ドロップ
		int criticalDropNum = dropNum * 2;
		// ダメージを受ける
		CCharaStatusBase::TakeDamage(criticalDamage, causer);
		// ゴミ袋を落とす
		DropTrashBag(criticalDropNum);
	}
	// 閉じていても通常のダメージを受ける
	else
	{
		// ダメージを受ける
		CCharaStatusBase::TakeDamage(damage, causer);
		// ゴミ袋を落とす
		DropTrashBag(dropNum);
	}

	// 死亡していなければ
	if (!IsDeath())
	{
		// 攻撃を加えた相手の方向へ向く
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// 被弾開始状態へ移行
		ChangeState(EState::eDamageStart);
	}
}

// 状態切り替え
void CTrashPlayer::ChangeState(EState state)
{
	// 既に同じ状態であれば、処理しない
	if (state == mState) return;

	// 攻撃中に他の状態に変わるとき
	// 攻撃終了処理を呼ぶ
	if (IsAttacking())
	{
		AttackEnd();
	}

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

#if _DEBUG
// 状態の文字列を取得
std::string CTrashPlayer::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "待機";
	case EState::eDamageStart:		return "被弾開始";
	case EState::eDamage:			return "被弾中";
	case EState::eDamageEnd:		return "被弾終了";
	case EState::eJumpStart:		return "ジャンプ開始";
	case EState::eJump:				return "ジャンプ中";
	case EState::eJumpEnd:			return "ジャンプ終了";
	case EState::eAttackStart:		return "攻撃開始";
	case EState::eAttack:			return "攻撃中";
	case EState::eAttackEnd:		return "攻撃終了";
	case EState::eCriticalStart:	return "クリティカル攻撃開始";
	case EState::eCritical:			return "クリティカル攻撃中";
	case EState::eCriticalEnd:		return "クリティカル攻撃終了";
	case EState::eOpenClose:		return "開閉中";
	case EState::eDeath:			return "死亡";
	}
	return "";
}
#endif
