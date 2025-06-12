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
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		mpModel = CResourceManager::Get<CModel>("FireBolt");
		break;
	case ESpellElementalType::eWind:
		mpModel = CResourceManager::Get<CModel>("WindBolt");
		break;
	case ESpellElementalType::eEarth:
		mpModel = CResourceManager::Get<CModel>("EarthBolt");
		break;
	case ESpellElementalType::eThunder:
		mpModel = CResourceManager::Get<CModel>("ThunderBolt");
		break;
	case ESpellElementalType::eWater:
		mpModel = CResourceManager::Get<CModel>("WaterBolt");
		break;
	case ESpellElementalType::eNeutral:
		mpModel = CResourceManager::Get<CModel>("NeutralBolt");
		break;
	}
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
	// 地形とプレイヤーと敵と攻撃判定と防御判定とだけ衝突判定
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eAttackCol,ELayer::eDefenseCol });
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
		// 目標の方向を向く
		LookAt(mpTarget->Position());

		// 移動方向の更新終了
		mStateStep++;
		break;
	}
	}
	// 移動を設定
	mMoveSpeed = mMoveDir * GetSpellStatus().speed * Times::DeltaTime();

	// 基底クラスの更新
	CSpellBase::UpdateShooting();
}