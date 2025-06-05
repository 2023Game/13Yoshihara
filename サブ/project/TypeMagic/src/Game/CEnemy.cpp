#include "CEnemy.h"
#include "CColliderCapsule.h"
#include "CBall.h"
#include "CPlayer.h"

// 体の半径と高さ
#define BODY_RADIUS 2.5f
#define BODY_HEIGHT 15.0f

// コンストラクタ
CEnemy::CEnemy()
	: CEnemyBase(0.0f, 0.0f, {}, 0.0f)
	, CSpellCaster(this)
	, mElapsedTime(0.0f)
{
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
	if (mElapsedTime > 5.0f)
	{
		CastStart(ESpellElementalType::eFire, ESpellShapeType::eBall);
		mElapsedTime = 0.0f;
	}
}

// コライダーを生成
void CEnemy::CreateCol()
{
	// 本体コライダ
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS / Scale().Y(), 0.0f),
		CVector(0.0f, BODY_HEIGHT / Scale().Y(), 0.0f),
		BODY_RADIUS
	);
	// フィールド,壁、オブジェクトとプレイヤーと攻撃とだけ衝突
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::ePlayer,
		ELayer::eAttackCol});
}
