#include "CTorch.h"
#include "CColliderCapsule.h"

// 松明の炎のオフセット座標
#define TORCH_FIRE_OFFSET_POS CVector(0.0f,9.0f,0.0f)

// コライダーの半径
#define RADIUS 5.0f

// 松明の縦の長さ
#define TORCH_HEIGHT 7.0f

// コンストラクタ
CTorch::CTorch()
	: CFire(TORCH_FIRE_OFFSET_POS)
{
	mpModel = CResourceManager::Get<CModel>("Torch");

	// コライダーを生成
	CreateCol();
}

// デストラクタ
CTorch::~CTorch()
{
}

// コライダーを生成
void CTorch::CreateCol()
{
	mpCol = new CColliderCapsule(
		this, ELayer::eObject,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, TORCH_HEIGHT, 0.0f),
		RADIUS, true
	);
	// オブジェクトとプレイヤーと接続オブジェクトの探知用とだけ衝突判定
	mpCol->SetCollisionLayers({ ELayer::eObject,ELayer::ePlayer,
		ELayer::eConnectSearch });
}
