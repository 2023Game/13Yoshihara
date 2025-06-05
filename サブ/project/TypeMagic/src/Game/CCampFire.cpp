#include "CCampFire.h"
#include "CColliderMesh.h"

// 炎のオフセット座標
#define FIRE_OFFSET_POS CVector(0.0f,3.0f,0.0f)
// 炎の大きさ
#define FIRE_SCALE 2.5f
// 炎の移動速度
#define FIRE_SPEED 10.0f
// 接続ターゲットのオフセット座標
#define TARGET_OFFSET_POS CVector(0.0f,15.0f,0.0f)

// コンストラクタ
CCampFire::CCampFire()
	: CFire(FIRE_OFFSET_POS, FIRE_SCALE, FIRE_SPEED, TARGET_OFFSET_POS)
	, CItemDrop()
{
	mpModel = CResourceManager::Get<CModel>("CampFire");

	// コライダーを生成
	CreateCol();
}

// デストラクタ
CCampFire::~CCampFire()
{
}

// コライダーを生成
void CCampFire::CreateCol()
{
	mpCol = new CColliderMesh(
		this, ELayer::eObject,
		CResourceManager::Get<CModel>("CampFire_Col"),
		true
	);
	// オブジェクトとプレイヤーと敵と
	// 接続オブジェクトの探知用とだけ衝突判定
	mpCol->SetCollisionLayers({ ELayer::eObject,ELayer::ePlayer,
		ELayer::eEnemy,ELayer::eConnectSearch });
}

// 燃えたときの処理
void CCampFire::Burning()
{
	// 落とすアイテムの座標を設定
	SetDropItemPos(Position());
	// アイテムを落とす
	Drop();
}
