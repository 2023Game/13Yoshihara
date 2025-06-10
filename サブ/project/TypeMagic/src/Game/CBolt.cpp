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
	// 待機状態
	ChangeState(EState::eIdle);
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
	// 地形とプレイヤーと敵と攻撃判定とだけ衝突判定
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eAttackCol });
}

// 待機中の更新
void CBolt::UpdateIdle()
{
	// 目標の方向を向く
	LookAt(mpTarget->Position());

	CSpellBase::UpdateIdle();
}

// 発射中の更新
void CBolt::UpdateShooting()
{
	switch (mStateStep)
	{
		// 目標への方向を移動方向に設定
	case 0:
	{
		mMoveDir = TargetDir();
		mMoveDir.Normalize();

		// 移動方向の更新終了
		mStateStep++;
		break;
	}
	}
	// 速度
	float speed = GetSpellStatus().speed;
	// 移動を設定
	mMoveSpeed = mMoveDir * speed * Times::DeltaTime();

	// 加速
	SetSpeed(speed + BOLT_ACCELERATE);

	// 基底クラスの更新
	CSpellBase::UpdateShooting();
}
