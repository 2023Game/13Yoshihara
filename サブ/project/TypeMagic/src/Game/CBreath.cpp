#include "CBreath.h"
#include "CColliderSphere.h"
#include "Maths.h"

// コライダーの半径
#define RADIUS 3.0f

// コンストラクタ
CBreath::CBreath(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBreath, owner, target, BREATH_DELETE_TIME)
{
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		mpModel = CResourceManager::Get<CModel>("FireBreath");
		break;
	case ESpellElementalType::eWind:
		mpModel = CResourceManager::Get<CModel>("WindBreath");
		break;
	case ESpellElementalType::eEarth:
		mpModel = CResourceManager::Get<CModel>("EarthBreath");
		break;
	case ESpellElementalType::eThunder:
		mpModel = CResourceManager::Get<CModel>("ThunderBreath");
		break;
	case ESpellElementalType::eWater:
		mpModel = CResourceManager::Get<CModel>("WaterBreath");
		break;
	case ESpellElementalType::eNeutral:
		mpModel = CResourceManager::Get<CModel>("NeutralBreath");
		break;
	}
	// コライダーを生成
	CreateCol();
	// 最初から発射状態
	ChangeState(EState::eShooting);
}

// デストラクタ
CBreath::~CBreath()
{
}

// コライダーを生成
void CBreath::CreateCol()
{
	mpSpellCol = new CColliderSphere(
		this, ELayer::eAttackCol,
		RADIUS
	);
	// 地形とプレイヤーと敵と攻撃判定と防御判定と呪文探知用とだけ衝突判定
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eAttackCol,ELayer::eDefenseCol,ELayer::eSpellSearch });
}

// 発射中の更新
void CBreath::UpdateShooting()
{
	switch (mStateStep)
	{
		// 移動方向を求める
	case 0:
		// 目標への方向
		mMoveDir = TargetDir();
		mMoveDir.Normalize();
		// 移動方向をランダムでぶらす
		mMoveDir.X(mMoveDir.X() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
		mMoveDir.Y(mMoveDir.Y() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
		mMoveDir.Z(mMoveDir.Z() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
		mMoveDir.Normalize();
		// 移動方向の更新を終了
		mStateStep++;
		break;
	}

	// 大きくする
	Scale(Scale() + BREATH_SCALE_UP_NUM);

	// 移動を設定
	mMoveSpeed = mMoveDir * GetSpellStatus().speed * Times::DeltaTime();

	// 基底クラスの更新
	CSpellBase::UpdateShooting();
}