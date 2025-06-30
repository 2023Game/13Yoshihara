#include "CShield.h"
#include "CSpellCaster.h"
#include "CColliderRectangle.h"
#include "Maths.h"
#include <cmath>

// コライダーの頂点
#define VERT_POS_1 CVector( 3.0f, 7.0f,-0.5f)
#define VERT_POS_2 CVector( 3.0f,-7.0f,-0.5f)
#define VERT_POS_3 CVector(-3.0f,-7.0f,-0.5f)
#define VERT_POS_4 CVector(-3.0f, 7.0f,-0.5f)

// コンストラクタ
CShield::CShield(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eShield, owner, target, SHIELD_DELETE_TIME)
	, mCurrentAngle(0.0f)
	, mAnglePos(CVector::zero)
{
	// 最小スケールに設定
	Scale(SHIELD_SCALE_MIN);
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		mpModel = CResourceManager::Get<CModel>("FireShield");
		break;
	case ESpellElementalType::eWind:
		mpModel = CResourceManager::Get<CModel>("WindShield");
		break;
	case ESpellElementalType::eEarth:
		mpModel = CResourceManager::Get<CModel>("EarthShield");
		break;
	case ESpellElementalType::eThunder:
		mpModel = CResourceManager::Get<CModel>("ThunderShield");
		break;
	case ESpellElementalType::eWater:
		mpModel = CResourceManager::Get<CModel>("WaterShield");
		break;
	case ESpellElementalType::eNeutral:
		mpModel = CResourceManager::Get<CModel>("NeutralShield");
		break;
	}
	// コライダーを生成
	CreateCol();
	// 発射状態
	ChangeState(EState::eShooting);
	// 親についていく
	SetParent(mpOwner);
	// シールドを張っている
	CSpellCaster* caster = dynamic_cast<CSpellCaster*>(mpOwner);
	caster->SetIsShield(true);
}

// デストラクタ
CShield::~CShield()
{
}

// 更新
void CShield::Update()
{
	// シールドを張っている
	CSpellCaster* caster = dynamic_cast<CSpellCaster*>(mpOwner);
	caster->SetIsShield(true);

	// 持ち主の方向を向く
	LookAt(mpOwner->Position());

	// 基底クラス更新
	CSpellBase::Update();
}

// コライダーを生成
void CShield::CreateCol()
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
void CShield::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// 大きくなっていく
	case 0:
		// 拡大
		Scale(Scale() + SHIELD_SCALE_UP_NUM);

		// 最大スケール以上になったら
		if (Scale().X() >= SHIELD_SCALE_MAX.X())
		{
			mElapsedTime = 0.0f;
			// 最大に設定
			Scale(SHIELD_SCALE_MAX);
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
		Scale(Scale() - SHIELD_SCALE_UP_NUM);
		// 最小スケール以下になったら
		if (Scale().X() <= SHIELD_SCALE_MIN.X())
		{
			// 削除
			Kill();
		}
		break;
	}
	// 角度を増加
	mCurrentAngle += SHIELD_ROT_SPEED * Times::DeltaTime();
	// 丸める
	if (mCurrentAngle >= 360.0f) mCurrentAngle -= 360.0f;

	// 回転角度
	float rad = mCurrentAngle * M_PI / 180.0f;

	// 回転後のオフセット座標
	CVector offsetPos =
		CVector(
			cos(rad) * SHIELD_ROT_RADIUS,
			0.0f,
			sin(rad) * SHIELD_ROT_RADIUS);

	// 座標更新
	Position(mpOwner->Position() + offsetPos);
}

// 削除
void CShield::Kill()
{
	// 張っていない
	CSpellCaster* caster = dynamic_cast<CSpellCaster*>(mpOwner);
	caster->SetIsShield(false);
	// 基底クラスの削除処理
	CTask::Kill();
}
