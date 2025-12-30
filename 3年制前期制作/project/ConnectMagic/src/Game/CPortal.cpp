#include "CPortal.h"
#include "CSceneManager.h"
#include "CConnectPointManager.h"
#include "CollisionData.h"
#include "CPhysicsManager.h"

// 回転速度
constexpr float ROT_SPEED = 200.0f;

// コライダーの半径
constexpr float RADIUS =	5.0f;

// 大きさ
constexpr float SIZE =		2.0f;

// センサーの範囲
const CVector HALF_EXTENTS = CVector(5.0f, 10.0f, 1.0f);

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
}

// コライダーを生成
void CPortal::CreateCol()
{
	CPhysicsManager::Instance()->CreateBoxSensor(
		this,
		HALF_EXTENTS,
		ELayer::ePortal,
		{ ELayer::ePlayer }
	);
}

void CPortal::OnCollision(const CollisionData& data)
{
	// センサー
	if (data.selfBody == GetSensor())
	{
		// 相手のobj
		CObjectBase* otherObj = static_cast<CObjectBase*>(data.otherBody->getUserPointer());

		if (otherObj == nullptr) return;

		// プレイヤーの場合
		if (otherObj->Tag() == ETag::ePlayer)
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

			// 次のステージへ
			CSceneManager::Instance()->LoadScene(NextScene);
		}
	}
}
