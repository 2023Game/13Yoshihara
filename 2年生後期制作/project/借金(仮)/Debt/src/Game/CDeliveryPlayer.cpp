#include "CDeliveryPlayer.h"
#include "CModel.h"
#include "CColliderCapsule.h"

#define TRUCK_HEIGHT	13.0f	// トラックの高さ
#define TRUCK_WIDTH		32.5f	// トラックの幅
#define TRUCK_RADIUS	12.5f	// トラックの半径
// 本体コライダ―のオフセット座標
#define BODY_COL_OFFSET_POS CVector(0.0f,0.0f,-3.0f)

#define EYE_HEIGHT 5.0f		// 視点の高さ

// コンストラクタ
CDeliveryPlayer::CDeliveryPlayer()
	: CObjectBase(ETag::eVehicle, ETaskPriority::eVehicle)
{
	mpModel = CResourceManager::Get<CModel>("DeliveryPlayer");

	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CDeliveryPlayer::~CDeliveryPlayer()
{
}

// 更新
void CDeliveryPlayer::Update()
{
}

// 衝突処理
void CDeliveryPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// 描画
void CDeliveryPlayer::Render()
{
	mpModel->Render(Matrix());
}

// コライダ―を生成
void CDeliveryPlayer::CreateCol()
{
	// 大きさの取得
	float scale = Scale().X();

	// 本体コライダ―
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
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
