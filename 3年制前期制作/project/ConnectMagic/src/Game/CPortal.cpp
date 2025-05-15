#include "CPortal.h"
#include "CSceneManager.h"

// 回転速度
#define ROT_SPEED 200.0f

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

// 衝突処理
void CPortal::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpCol)
	{
		// プレイヤーの場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// 次のステージへ
			CSceneManager::Instance()->LoadScene(EScene::eGameTest);
		}
	}
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
