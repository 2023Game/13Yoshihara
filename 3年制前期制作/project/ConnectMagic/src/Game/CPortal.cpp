#include "CPortal.h"
#include "CSceneManager.h"
#include "CColliderSphere.h"

// 回転速度
#define ROT_SPEED 200.0f

// コライダーの半径
#define RADIUS 5.0f

// 大きさ
#define SIZE 2.0f

// コンストラクタ
CPortal::CPortal()
	: CImage3D("Effect/Portal.png",
		ETag::ePortal, ETaskPriority::eEffect, 0,
		ETaskPauseType::eGame)
{
	// 裏面描画する
	SetBackFace(true);
	// コライダーを生成
	CreateCol();

	// 回転する
	SetRotate(true);
	// 回転速度を設定
	SetRotSpeed(ROT_SPEED);

	// 大きさを設定
	SetSize(GetSize() * SIZE);
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
			// シーン管理クラス
			CSceneManager* SceneMgr = CSceneManager::Instance();
			// 現在のシーン
			EScene CurrentScene = SceneMgr->GetCurrentScene();
			// 次のシーン
			EScene NextScene = EScene::eMap1;
			// 現在のシーンから次のシーンを決定
			switch (CurrentScene)
			{
			case EScene::eMap1:		NextScene = EScene::eMap2;  break;
			case EScene::eMap2:		NextScene = EScene::eMap3;	break;
			case EScene::eMap3:		NextScene = EScene::eMap1;	break;
			case EScene::eGameTest:	NextScene = EScene::eGameTest;	break;
			}

			// 次のステージへ
			CSceneManager::Instance()->LoadScene(NextScene);
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
