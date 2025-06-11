#include "CReflector.h"
#include "CColliderRectangle.h"
#include "Maths.h"
#include <cmath>
#include "CSpellCaster.h"

// コライダーの頂点
#define VERT_POS_1 CVector( 10.0f, 10.0f,-0.5f)
#define VERT_POS_2 CVector( 10.0f,-10.0f,-0.5f)
#define VERT_POS_3 CVector(-10.0f,-10.0f,-0.5f)
#define VERT_POS_4 CVector(-10.0f, 10.0f,-0.5f)

// コンストラクタ
CReflector::CReflector(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eReflector, owner, target, REFLECTOR_DELETE_TIME)
{
	// 最小スケールに設定
	Scale(REFLECTOR_SCALE_MIN);
	mpModel = CResourceManager::Get<CModel>("NeutralReflector");
	// コライダーを生成
	CreateCol();
	// 発射状態
	ChangeState(EState::eShooting);
	// 親についていく
	SetParent(mpOwner);
}

// デストラクタ
CReflector::~CReflector()
{
}

// 更新
void CReflector::Update()
{
	// 持ち主の方向を向く
	LookAt(mpOwner->Position());

	// 基底クラス更新
	CSpellBase::Update();
}

// コライダーを生成
void CReflector::CreateCol()
{
	mpSpellCol = new CColliderRectangle(
		this, ELayer::eDefenseCol,
		VERT_POS_1,
		VERT_POS_2,
		VERT_POS_3,
		VERT_POS_4,
		true
	);
	// 攻撃判定とだけ衝突判定
	mpSpellCol->SetCollisionLayers({ ELayer::eAttackCol });
}

// 発射中の更新
void CReflector::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// 大きくなっていく
	case 0:
		// 拡大
		Scale(Scale() + REFLECTOR_SCALE_UP_NUM);

		// 最大スケール以上になったら
		if (Scale().X() >= REFLECTOR_SCALE_MAX.X())
		{
			mElapsedTime = 0.0f;
			// 最大に設定
			Scale(REFLECTOR_SCALE_MAX);
			// 拡大終了
			mStateStep++;
		}
		break;
		// 削除時間が経過したら次へ
	case 1:
		if (mElapsedTime > mDeleteTime)
		{
			mStateStep++;
		}
		break;
		// 最小スケールになったら削除
	case 2:
		// 縮小
		Scale(Scale() - REFLECTOR_SCALE_UP_NUM);
		// 最小スケール以下になったら
		if (Scale().X() <= REFLECTOR_SCALE_MIN.X())
		{
			// 削除
			Kill();
		}
		break;
	}
}