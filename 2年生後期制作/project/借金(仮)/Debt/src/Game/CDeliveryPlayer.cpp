#include "CDeliveryPlayer.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CInput.h"
#include "CDeliveryItem.h"
#include "CDeliveryField.h"

#define TRUCK_HEIGHT	13.0f	// トラックの高さ
#define TRUCK_WIDTH		32.5f	// トラックの幅
#define TRUCK_RADIUS	12.5f	// トラックの半径
// 本体コライダ―のオフセット座標
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-3.0f)

#define EYE_HEIGHT 5.0f		// 視点の高さ

#define ROTATE_SPEED 3.0f	// 回転速度

// 初期の方向
#define ROTATION CVector(0.0f,180.0f,0.0f)

// 車線変更時の車線のオフセット座標
#define CHANGE_ROAD_OFFSET_POS_L CVector(-60.0f,0.0f,0.0f)
#define CHANGE_ROAD_OFFSET_POS_R CVector( 60.0f,0.0f,0.0f)
// 閾値
#define CHANGE_ROAD_THRESHOLD 0.1f

// 弾丸のオフセット座標
#define BULLET_OFFSET_POS_L  CVector(-20.0f,14.75f,10.0f)	// 左
#define BULLET_OFFSET_POS_R  CVector( 19.0f,14.75f,10.0f)	// 右
#define BULLET_OFFSET_POS_B1 CVector( 6.5f, 14.75f,41.0f)	// 後ろ1
#define BULLET_OFFSET_POS_B2 CVector(-7.5f, 14.75f,41.0f)	// 後ろ2
// 弾丸の方向
#define BULLET_ROT_LR	CVector(0.0f,90.0f,0.0f) // 左右

// ダメージ後の無敵時間
#define INVINCIBLE_TIME 1.0f
// 点滅間隔
#define HIT_FLASH_INTERVAL 0.25f

// コンストラクタ
CDeliveryPlayer::CDeliveryPlayer()
	: CPlayerBase()
	, CDeliveryPlayerStatus()
	, mInvincibleTime(0.0f)
	, mHitFlashTime(0.0f)
	, mTargetPos(CVector::zero)
	, mDeliveryNum(0)
	, mDestroyEnemyNum(0)
{
	// 移動状態
	ChangeState(EState::eMove);
	// 方向を設定
	Rotation(ROTATION);
	// 移動方向を向かない
	mIsMoveDir = false;
	// 重力無効
	mIsGravity = false;
	mpModel = CResourceManager::Get<CModel>("DeliveryPlayer");

	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CDeliveryPlayer::~CDeliveryPlayer()
{
}

// ダメージを受ける
void CDeliveryPlayer::TakeDamage(int damage, CObjectBase* causer)
{
	// ダメージを受けている時は処理しない
	if (IsDamaging()) return;
	// ダメージを受けている
	mIsDamage = true;
	// ダメージを受ける
	CCharaStatusBase::TakeDamage(damage, causer);
}

// 更新
void CDeliveryPlayer::Update()
{
	switch (mState)
	{
	case EState::eMove:			UpdateMove();			break;
	case EState::eChangeRoad:	UpdateChangeRoad();		break;
	case EState::eDeath:		UpdateDeath();			break;
	}

	// キー入力可能
	ActionInput();

	// ダメージを受けていたら点滅する
	HitFlash();
	
	// 基底クラスの更新
	CPlayerBase::Update();

#if _DEBUG
	CDebugPrint::Print("PlayerState：%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("PlayerHP：%d\n", GetHp());
#endif
}

// 衝突処理
void CDeliveryPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CPlayerBase::Collision(self, other, hit);
}

// 描画
void CDeliveryPlayer::Render()
{
	mpModel->Render(Matrix());
}

// 配達した数を1増やす
void CDeliveryPlayer::IncreaseDeliveryNum()
{
	mDeliveryNum++;
}

// 配達した数を取得する
int CDeliveryPlayer::GetDeliveryNum() const
{
	return mDeliveryNum;
}

// 壊したトラックの数を1増やす
void CDeliveryPlayer::IncreaseDestroyEnemyNum()
{
	mDestroyEnemyNum++;
}

// 壊したトラックの数を取得する
int CDeliveryPlayer::GetDestroyEnemyNum() const
{
	return mDestroyEnemyNum;
}

// 状態切り替え
void CDeliveryPlayer::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// コライダ―を生成
void CDeliveryPlayer::CreateCol()
{
	// 本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// 地形、敵、障害物、発射物の攻撃判定
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::eEnemy,
		ETag::eObstruction,ETag::eBullet });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::eEnemy,
		ELayer::eObstruction,ELayer::eAttackCol });
}

#if _DEBUG
// 状態の文字列を取得
std::string CDeliveryPlayer::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:			return "移動";
	case EState::eChangeRoad:	return "車線変更";
	case EState::eDeath:		return "死亡";
	}
	return "";
}
#endif

// 移動の更新処理
void CDeliveryPlayer::UpdateMove()
{
	// 使用しないので
	// 目的地を自分に設定しておく
	mTargetPos = Position();

	mMoveSpeed = CVector::zero;
	CVector forward = CVector::Slerp(VectorZ(), CVector::back, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 横移動は0にする
	move.X(0.0f);
	// 移動がプラス方向なら
	if (move.Z() > 0.0f)
	{
		if (Position().Z() >= ROAD_Z_AREA_P)
		{
			// Z座標を範囲に設定して
			Position(Position().X(), Position().Y(), ROAD_Z_AREA_P);
			// Z移動を0
			move.Z(0.0f);
		}
	}
	// 移動がマイナス方向なら
	else if (move.Z() < 0.0f)
	{
		// マイナス方向の範囲外なら
		if (Position().Z() <= ROAD_Z_AREA_M)
		{
			// Z座標を範囲に設定して
			Position(Position().X(), Position().Y(), ROAD_Z_AREA_M);
			// Z移動を0
			move.Z(0.0f);
		}
	}
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		// 基礎移動速度で移動
		mMoveSpeed += move * GetBaseMoveSpeed() * Times::DeltaTime();
	}
}

// 車線変更の更新処理
void CDeliveryPlayer::UpdateChangeRoad()
{
	mMoveSpeed = CVector::zero;

	// 目的地へ移動
	MoveTo(mTargetPos, GetBaseMoveSpeed(), ROTATE_SPEED);

	// 自分のX座標が閾値内に入ったら
	if (Position().X() <= mTargetPos.X() + CHANGE_ROAD_THRESHOLD &&
		Position().X() >= mTargetPos.X() - CHANGE_ROAD_THRESHOLD)
	{
		// 移動状態へ
		ChangeState(EState::eMove);
		return;
	}
}

// 死亡の更新処理
void CDeliveryPlayer::UpdateDeath()
{
	// 移動をゼロ
	mMoveSpeedY = 0.0f;
	mMoveSpeed = CVector::zero;
	// 衝突無効
	SetEnableCol(false);
	// ゲームシーンでプレイヤーが死亡したことによる
	// ゲーム終了を取得するための変数設定
	SetGameEnd(true);
}

// 死亡
void CDeliveryPlayer::Death()
{
	// 死亡状態へ
	ChangeState(EState::eDeath);
}

// 指定した位置まで移動する
bool CDeliveryPlayer::MoveTo(const CVector& targetPos, float speed, float rotateSpeed)
{
	// 目的地までのベクトルを求める
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);
	// 移動方向ベクトルを求める
	CVector moveDir = vec.Normalized();

	// 徐々に移動方向へ向ける
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// 現在の正面方向
		moveDir,	// 移動方向
		rotateSpeed * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// 今回の移動距離を求める
	float moveDist = speed * Times::DeltaTime();
	// 目的地までの残りの距離を求める
	float remainDist = vec.Length();
	// 残りの距離が移動距離より短い
	if (remainDist <= moveDist)
	{
		pos = CVector(targetPos.X(), pos.Y(), targetPos.Z());
		Position(pos);
		return true;	// 目的地に到着したので、trueを返す
	}

	// 残りの距離が移動距離より長い場合は
	// 移動距離分目的地へ移動
	pos += moveDir * moveDist;
	Position(pos);

	// 目的地には到着しなかった
	return false;
}

// アクションのキー入力
void CDeliveryPlayer::ActionInput()
{
	if (CInput::PushKey('1'))
	{
		TakeDamage(1, nullptr);
	}
	// Aキーで、左へ移動
	if (CInput::PushKey('A'))
	{
		// 一番左の車道なら処理しない
		if (mTargetPos.X() <= -ROAD_X_AREA + 30.0f) return;
		// 目的地を設定
		mTargetPos = mTargetPos + CHANGE_ROAD_OFFSET_POS_L;
		// 車線変更状態へ
		ChangeState(EState::eChangeRoad);
		return;
	}
	// Dキーで、右へ移動
	else if (CInput::PushKey('D'))
	{
		// 一番右の車道なら処理しない
		if (mTargetPos.X() >= ROAD_X_AREA - 30.0f) return;
		// 目的地を設定
		mTargetPos = mTargetPos + CHANGE_ROAD_OFFSET_POS_R;
		// 車線変更状態へ
		ChangeState(EState::eChangeRoad);
		return;
	}

	//// 車線変更中は射撃できない
	//if (mState == EState::eChangeRoad) return;

	// 左クリックで、左方向へ射撃
	if (CInput::PushKey(VK_LBUTTON))
	{
		// 配達物を生成
		CDeliveryItem* item = new CDeliveryItem(this);
		// 座標を設定
		item->Position(Position() + BULLET_OFFSET_POS_L);
		// 回転を設定
		item->Rotation(BULLET_ROT_LR);
		// 移動速度
		float moveSpeedX = GetBaseMoveSpeed() * Times::DeltaTime();
		// 移動を設定
		item->SetMoveSpeed(-VectorX() * moveSpeedX);
	}
	// 右クリックで、右方向へ射撃
	if (CInput::PushKey(VK_RBUTTON))
	{
		// 配達物を生成
		CDeliveryItem* item = new CDeliveryItem(this);
		// 座標を設定
		item->Position(Position() + BULLET_OFFSET_POS_R);
		// 回転を設定
		item->Rotation(BULLET_ROT_LR);
		// 移動速度
		float moveSpeedX = GetBaseMoveSpeed() * Times::DeltaTime();
		// 移動を設定
		item->SetMoveSpeed(VectorX() * moveSpeedX);
	}
	// スペースキーで、後方へ射撃
	if (CInput::PushKey(VK_SPACE))
	{
		// 配達物1を生成
		CDeliveryItem* item1 = new CDeliveryItem(this);
		// 座標を設定
		item1->Position(Position() + BULLET_OFFSET_POS_B1);
		// 移動を設定
		item1->SetMoveSpeed(-VectorZ() * GetBaseMoveSpeed() * Times::DeltaTime());

		// 配達物2を生成
		CDeliveryItem* item2 = new CDeliveryItem(this);
		// 座標を設定
		item2->Position(Position() + BULLET_OFFSET_POS_B2);
		// 移動を設定
		item2->SetMoveSpeed(-VectorZ() * GetBaseMoveSpeed() * Times::DeltaTime());
	}
}

// ダメージの点滅と無敵時間の処理
void CDeliveryPlayer::HitFlash()
{
	if (IsDamaging())
	{
		// 点滅間隔が経過したら
		if (mHitFlashTime > HIT_FLASH_INTERVAL)
		{
			mHitFlashTime -= HIT_FLASH_INTERVAL;
			// 描画するかを反転
			SetShow(!IsShow());
		}
		// 無敵時間が経過したら
		if (mInvincibleTime > INVINCIBLE_TIME)
		{
			// 描画する
			SetShow(true);
			// 衝突判定有効
			SetEnableCol(true);
			// ダメージを受けていない
			mIsDamage = false;
			// 計測用の変数をリセット
			mHitFlashTime = 0.0f;
			mInvincibleTime = 0.0f;
		}
		mHitFlashTime += Times::DeltaTime();
		mInvincibleTime += Times::DeltaTime();
	}
}
