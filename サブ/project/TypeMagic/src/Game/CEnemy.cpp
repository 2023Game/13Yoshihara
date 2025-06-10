#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CBall.h"
#include "CPlayer.h"

// 体の半径
#define BODY_RADIUS 4.0f

// コンストラクタ
CEnemy::CEnemy()
	: CEnemyBase(0.0f, 0.0f, {}, 0.0f)
	, CSpellCaster(this)
	, mElapsedTime(0.0f)
{
	// 重力無効
	mIsGravity = false;
	// 移動方向を向かない
	mIsMoveDir = false;

	// コライダーを生成
	CreateCol();
}

// デストラクタ
CEnemy::~CEnemy()
{
}

// 更新
void CEnemy::Update()
{
	// プレイヤーの方向を向き続ける
	CVector lookPos = CPlayer::Instance()->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// 敵の基底クラスの更新
	CEnemyBase::Update();
	// 呪文詠唱クラスの更新
	CSpellCaster::Update();

	mElapsedTime += Times::DeltaTime();
	if (mElapsedTime > 2.0f)
	{
		//CastStart(ESpellElementalType::eFire, ESpellShapeType::eBall);
		mElapsedTime = 0.0f;
	}

#if _DEBUG
	CDebugPrint::Print("EnemyHit:%d\n", mHitCount);
#endif
}

// コライダーを生成
void CEnemy::CreateCol()
{
	// 本体コライダ
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eEnemy,
		BODY_RADIUS
	);
	// フィールド,壁、オブジェクトとプレイヤーと攻撃とだけ衝突
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::ePlayer,
		ELayer::eAttackCol});
}
