#include "CPortal.h"

// 回転速度
#define ROT_SPEED 60.0f * 2.0f

// コライダーの半径
#define RADIUS 5.0f

// コンストラクタ
CPortal::CPortal()
	: CBillBoardImage("Effect/Portal.png", ETag::ePortal
		, ETaskPauseType::eGame)
{
	// コライダーを生成
	CreateCol();

	// 回転する
	SetRotate(true);
	// 回転速度を設定
	SetRotSpeed(ROT_SPEED);
}

// デストラクタ
CPortal::~CPortal()
{
	SAFE_DELETE(mpCol);
}

// コライダーを生成
void CPortal::CreateCol()
{
	mpCol = new CColliderSphere
	(
		this, ELayer::ePortal,
		RADIUS, true
	);
	// プレイヤーとだけ衝突判定
	mpCol->SetCollisionLayers({ ELayer::ePlayer });
}
