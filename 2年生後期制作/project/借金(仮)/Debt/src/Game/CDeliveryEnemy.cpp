#include "CDeliveryEnemy.h"
#include "CModel.h"
#include "CColliderCapsule.h"

#define TRUCK_HEIGHT	13.0f	// トラックの高さ
#define TRUCK_WIDTH		32.5f	// トラックの幅
#define TRUCK_RADIUS	12.5f	// トラックの半径
// 本体コライダ―のオフセット座標
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-3.0f)

#define EYE_HEIGHT 5.0f		// 視点の高さ

#define MAX_HP 10						// 最大HP
#define BASE_MOVE_SPEED 7.5f * 60.0f	// 移動速度
#define JUMP_SPEED 1.5f					// ジャンプ速度
#define KNOCKBACK_SPEED 0.375f * 2.0f	// ノックバック速度
#define ATTACK_POWER 1					// 攻撃力

// 初期の方向
#define ROTATION CVector(0.0f,180.0f,0.0f)

// コンストラクタ
CDeliveryEnemy::CDeliveryEnemy()
	: CEnemyBase
	(
		0.0f,0.0f,
		{},
		EYE_HEIGHT
	)
	, CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED, ATTACK_POWER)

{
	// 方向を設定
	Rotation(ROTATION);
	mpModel = CResourceManager::Get<CModel>("DeliveryEnemy");

	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CDeliveryEnemy::~CDeliveryEnemy()
{
}

// 更新
void CDeliveryEnemy::Update()
{
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed() * Times::DeltaTime();
	// 基底クラスの更新
	CEnemyBase::Update();
}

// 衝突処理
void CDeliveryEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 基底クラスの衝突処理
	CEnemyBase::Collision(self, other, hit);
}

// 描画
void CDeliveryEnemy::Render()
{
	mpModel->Render(Matrix());
}

// コライダ―を生成
void CDeliveryEnemy::CreateCol()
{
	// 本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eEnemy,
		CVector(0.0f, TRUCK_HEIGHT, TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f, TRUCK_HEIGHT, -TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// 地形、プレイヤー、障害物、発射物の攻撃判定
	// と衝突判定をする
	mpBodyCol->SetCollisionTags({ ETag::eField,ETag::ePlayer,
		ETag::eObstruction,ETag::eBullet });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,ELayer::ePlayer,
		ELayer::eObstruction,ELayer::eAttackCol });
}
