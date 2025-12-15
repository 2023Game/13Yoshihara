#include "CPortal.h"
#include "CSceneManager.h"
#include "CConnectPointManager.h"
#include "CColliderSphere.h"

// 回転速度
constexpr float ROT_SPEED = 200.0f;

// コライダーの半径
constexpr float RADIUS =	5.0f;

// 大きさ
constexpr float SIZE =		2.0f;

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
			EScene NextScene = EScene::eClear;
			// 現在のシーンから次のシーンを決定
			switch (CurrentScene)
			{
			case EScene::eGame:		NextScene = EScene::eClear;	break;
			case EScene::eGameTest:	NextScene = EScene::eGameTest;	break;
			}
			// 衝突判定を行うコライダーをリセット
			CConnectPointManager::Instance()->ResetCollider();

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
