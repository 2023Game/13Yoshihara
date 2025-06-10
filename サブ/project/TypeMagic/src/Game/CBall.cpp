#include "CBall.h"
#include "CColliderSphere.h"
#include "SpellDefine.h"

// コライダーの半径
#define RADIUS 1.0f

// コンストラクタ
CBall::CBall(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBall, owner, target, BALL_DELETE_TIME)
{
	mpModel = CResourceManager::Get<CModel>("FireBall");
	// コライダーを生成
	CreateCol();
	// 待機状態
	ChangeState(EState::eIdle);
}

// デストラクタ
CBall::~CBall()
{
}

// コライダーを生成
void CBall::CreateCol()
{
	mpSpellCol = new CColliderSphere(
		this, ELayer::eAttackCol,
		RADIUS
	);
	// 地形とプレイヤーと敵と攻撃判定とだけ衝突判定
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eAttackCol });
}

// 発射中の更新
void CBall::UpdateShooting()
{
	switch (mStateStep)
	{
		// 移動方向の更新
	case 0:
	{
		mMoveSpeed = CVector::zero;
		// 相手への方向
		mMoveDir = TargetDir();
		// 相手への距離
		float dist = mMoveDir.Length();
		mMoveDir.Normalize();

		// 追跡する距離より近くなったら
		if (dist < BALL_CHASE_END_DIST)
		{
			// 残りの消滅までの時間が
			// 追跡終了後の消滅時間より長ければ
			if (mDeleteTime - mElapsedTime > BALL_AFTER_CHASE_END_DELETE_TIME)
			{
				// 追跡終了後の消滅時間を適用
				mDeleteTime = BALL_AFTER_CHASE_END_DELETE_TIME;
				mElapsedTime = 0.0f;
			}
			// 移動方向の更新を終了
			mStateStep++;
		}
		break;
	}
	}

	// 移動速度
	float speed = GetSpellStatus().speed;
	// 移動速度を設定
	mMoveSpeed = mMoveDir * speed * Times::DeltaTime();

	// 加速
	SetSpeed(speed + BALL_ACCELERATE);

	// 基底クラスの更新
	CSpellBase::UpdateShooting();
}
