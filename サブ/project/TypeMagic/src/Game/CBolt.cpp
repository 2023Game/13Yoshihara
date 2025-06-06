#include "CBolt.h"
#include "CColliderCapsule.h"

// コライダーの幅
#define WIDTH 8.0f
// コライダーの半径
#define RADIUS 0.5f

// コンストラクタ
CBolt::CBolt(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBolt, owner, target, BOLT_DELETE_TIME)
{
	mpModel = CResourceManager::Get<CModel>("FireBolt");
	// コライダーを生成
	CreateCol();
}

// デストラクタ
CBolt::~CBolt()
{
}

// コライダーを生成
void CBolt::CreateCol()
{
	mpSpellCol = new CColliderCapsule(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, WIDTH),
		RADIUS
	);
	// 地形とプレイヤーと敵とだけ衝突判定
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy });
}

// 待機中の更新
void CBolt::UpdateIdle()
{
	CVector targetPos = mpTarget->Position();
	targetPos.Y(targetPos.Y() + TARGET_HEIGHT);
	// 目標の方向を向く
	LookAt(targetPos);

	CSpellBase::UpdateIdle();
}

// 発射中の更新
void CBolt::UpdateShooting()
{
	switch (mStateStep)
	{
		// 目標方向を求める
	case 0:
	{
		// 狙う高さを足す
		CVector targetPos = mpTarget->Position();
		targetPos.Y(targetPos.Y() + TARGET_HEIGHT);
		// 目標への方向
		mMoveDir = targetPos - Position();
		mMoveDir.Normalize();

		// 次へ
		mStateStep++;
		break;
	}
		
		// 目標方向へ移動
	case 1:
		float speed = GetSpellStatus().speed;

		// 目標方向に向かって移動
		mMoveSpeed = mMoveDir * speed * Times::DeltaTime();

		// 加速
		SetSpeed(speed + BOLT_ACCELERATE);
		break;
	}

	// 基底クラスの更新
	CSpellBase::UpdateShooting();
}
