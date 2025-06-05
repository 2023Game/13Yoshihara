#include "CBall.h"
#include "CColliderSphere.h"

// コライダーの半径
#define RADIUS 1.0f

// 狙う高さ
#define HEIGHT 7.5f

// 加速
#define ACCELERATE 1.0f

// 追跡時間
#define CHASE_TIME 2.0f

// コンストラクタ
CBall::CBall(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target, int generateNum)
	: CSpellBase(elemental, ESpellShapeType::eBall, owner, target)
	, mGenerateNum(generateNum)
{
	mpModel = CResourceManager::Get<CModel>("FireBall");
	// コライダーを生成
	CreateCol();
}

// デストラクタ
CBall::~CBall()
{
}

// 更新処理
void CBall::Update()
{
	// 発射中以外なら持ち主についていく
	if (mState != EState::eShooting)
	{
		SetParent(mpOwner);
	}
	// 発射中ならついていかない
	else
	{
		SetParent(nullptr);
	}
	// 基底クラスの更新処理
	CSpellBase::Update();
}

// 衝突処理
void CBall::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpSpellCol)
	{
		// 相手が地形なら
		if (other->Layer() == ELayer::eGround ||
			other->Layer() == ELayer::eWall ||
			other->Layer() == ELayer::eObject)
		{
			// 削除
			Kill();
		}
		// 相手がプレイヤーなら
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 削除
			Kill();
		}
		// 相手が敵なら
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 削除
			Kill();
		}
	}
}

// コライダーを生成
void CBall::CreateCol()
{
	mpSpellCol = new CColliderSphere(
		this, ELayer::eAttackCol,
		RADIUS
	);
	// 地形とプレイヤーと敵とだけ衝突判定
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy });
}

// 発射中の更新
void CBall::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	// 追跡時間が終了したら
	if (mElapsedTime > CHASE_TIME)
	{
		// 削除
		Kill();
	}

	mMoveSpeed = CVector::zero;
	// 相手の位置
	CVector opponentPos = mpTarget->Position();
	opponentPos.Y(opponentPos.Y() + HEIGHT);
	// 相手への方向
	CVector dir = opponentPos -Position();
	dir.Normalize();
	// 移動速度
	float speed = GetSpellStatus().speed;
	// 移動速度を設定
	mMoveSpeed = dir * GetSpellStatus().speed * Times::DeltaTime();

	// 加速
	SetSpeed(speed + ACCELERATE);
}
