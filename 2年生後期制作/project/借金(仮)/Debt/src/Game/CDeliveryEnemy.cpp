#include "CDeliveryEnemy.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CDeliveryPlayer.h"
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
#define CHANGE_ROAD_OFFSET_POS_L CVector(-40.0f,0.0f,-10.0f)
#define CHANGE_ROAD_OFFSET_POS_R CVector( 40.0f,0.0f,-10.0f)
// 閾値
#define CHANGE_ROAD_THRESHOLD 0.1f

// 弾丸のオフセット座標
#define BULLET_OFFSET_POS_L  CVector(-20.0f,14.75f,10.0f)	// 左
#define BULLET_OFFSET_POS_R  CVector( 19.0f,14.75f,10.0f)	// 右
#define BULLET_OFFSET_POS_B CVector( 6.5f, 14.75f,41.0f)	// 後ろ1
#define BULLET_OFFSET_POS_B2 CVector(-7.5f, 14.75f,41.0f)	// 後ろ2
// 弾丸の方向
#define BULLET_ROT_LR	CVector(0.0f,90.0f,0.0f) // 左右

// ダメージ後の無敵時間
#define INVINCIBLE_TIME 1.0f
// 点滅間隔
#define HIT_FLASH_INTERVAL 0.25f

// 消滅するZ座標
#define DELETE_POSZ 300.0f

// コンストラクタ
CDeliveryEnemy::CDeliveryEnemy()
	: CEnemyBase
	(
		0.0f,0.0f,
		{},
		EYE_HEIGHT
	)
	, CDeliveryEnemyStatus()
	, mInvincibleTime(0.0f)
	, mHitFlashTime(0.0f)

{
	// 移動状態
	ChangeState(EState::eMove);
	// 方向を設定
	Rotation(ROTATION);
	// 移動方向を向かない
	mIsMoveDir = false;
	// 重力無効
	mIsGravity = false;
	mpModel = CResourceManager::Get<CModel>("DeliveryEnemy");

	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CDeliveryEnemy::~CDeliveryEnemy()
{
}

// ダメージを受ける
void CDeliveryEnemy::TakeDamage(int damage, CObjectBase* causer, bool isShot)
{
	// ダメージを受けている時は処理しない
	if (IsDamaging()) return;

	// 死亡状態なら、ダメージを受けない
	if (mState == EState::eDeath) return;

	// 自分の射撃による減少ではない場合
	if (!isShot)
	{
		// ダメージを受けている
		mIsDamage = true;
	}

	// Hpが0の時攻撃を受けたら死亡
	if (GetHp() == 0)
	{
		Death();
		return;
	}

	// Hpをダメージ分減らす
	SetHp(-damage);
}

// 更新
void CDeliveryEnemy::Update()
{
	switch (mState)
	{
	case EState::eMove:			UpdateMove();			break;
	case EState::eChangeRoad:	UpdateChangeRoad();		break;
	case EState::eDeath:		UpdateDeath();			break;
	}

	// ダメージを受けていたら点滅する
	HitFlash();

	// 基底クラスの更新
	CEnemyBase::Update();

#if _DEBUG
	CDebugPrint::Print("EnemyState：%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("EnemyHp：%d\n", GetHp());
	CDebugPrint::Print("EnemyTargetPos：%f,%f,%f\n", mTargetPos.X(), mTargetPos.Y(), mTargetPos.Z());
#endif
}

// 衝突処理
void CDeliveryEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CEnemyBase::Collision(self, other, hit);
	// 本体コライダ―
	if (self == mpBodyCol)
	{
		// プレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(other->Owner());
			player->TakeDamage(GetAttackPower(), this);
		}
	}
}

// 描画
void CDeliveryEnemy::Render()
{
	mpModel->Render(Matrix());
}

// 有効無効を切り替える
void CDeliveryEnemy::SetOnOff(bool setOnOff)
{
	// 有効無効を設定
	SetEnable(setOnOff);
	SetShow(setOnOff);
}

// 現在の車道を設定
void CDeliveryEnemy::SetRoadType(ERoadType roadType)
{
	mRoadType = roadType;
}

// 現在の車道を取得
ERoadType CDeliveryEnemy::GetRoadType() const
{
	return ERoadType();
}

// 状態切り替え
void CDeliveryEnemy::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

#if _DEBUG
// 状態の文字列を取得
std::string CDeliveryEnemy::GetStateStr(EState state) const
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
void CDeliveryEnemy::UpdateMove()
{
	// 今いる道のX座標
	float targetPosX = 0.0f;
	switch (mRoadType)
	{
	case ERoadType::eLeft1:
		targetPosX = ROAD_LEFT1_POSX;
		break;
	case ERoadType::eLeft2:
		targetPosX = ROAD_LEFT2_POSX;
		break;
	case ERoadType::eRight1:
		targetPosX = ROAD_RIGHT1_POSX;
		break;
	case ERoadType::eRight2:
		targetPosX = ROAD_RIGHT2_POSX;
		break;
	}
	// 使用しないので
	// 目的地を自分に設定しておく
	mTargetPos = CVector(targetPosX, Position().Y(), Position().Z());
	Position(mTargetPos);
}

// 車線変更の更新処理
void CDeliveryEnemy::UpdateChangeRoad()
{
	mMoveSpeed = CVector::zero;

	// 目的地へ移動
	MoveTo(mTargetPos, GetBaseMoveSpeed(), ROTATE_SPEED);

	// 自分のX座標が閾値内に入ったら
	if (Position().X() <= mTargetPos.X() + CHANGE_ROAD_THRESHOLD &&
		Position().X() >= mTargetPos.X() - CHANGE_ROAD_THRESHOLD)
	{
		// 目的地の車道のタイプに変更
		mRoadType = mTargetRoadType;
		// 移動状態へ
		ChangeState(EState::eMove);
		return;
	}
}

// 死亡の更新処理
void CDeliveryEnemy::UpdateDeath()
{
	float moveSpeed = GetBaseMoveSpeed() * 0.2f * Times::DeltaTime();
	mMoveSpeed = -VectorZ() * moveSpeed;
	if (Position().Z() > DELETE_POSZ)
	{
		// 無効
		SetOnOff(false);
	}
}

// 死亡
void CDeliveryEnemy::Death()
{
	// 壊した敵の数を増やす
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
	player->IncreaseDestroyEnemyNum();
	// 攻撃を受けていない
	mIsDamage = false;
	// 死亡状態へ
	ChangeState(EState::eDeath);
}

// コライダ―を生成
void CDeliveryEnemy::CreateCol()
{
	// 本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eEnemy,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// 地形、プレイヤー、障害物、発射物の攻撃判定、アイテム
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::ePlayer,
		ETag::eObstruction,ETag::eBullet,ETag::eItem });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::ePlayer,
		ELayer::eObstruction,ELayer::eAttackCol,ELayer::eItem });
}

// 指定した位置まで移動する
bool CDeliveryEnemy::MoveTo(const CVector& targetPos, float speed, float rotateSpeed)
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

// ダメージの点滅と無敵時間の処理
void CDeliveryEnemy::HitFlash()
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
