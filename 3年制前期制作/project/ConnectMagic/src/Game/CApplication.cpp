#include "CApplication.h"
//OpenGL
#include "glut.h"
#include "CTaskManager.h"
#include "CSoundManager.h"
#include "CBGMManager.h"
#include "CSceneManager.h"
#include "CGamePause.h"
#include "CFade.h"
#include "FadeType.h"
#include "CConnectPointManager.h"
#include "CPhysicsManager.h"

CApplication::~CApplication()
{
}

void CApplication::Start()
{
#if _DEBUG
	// デバッグカメラを作成
	CDebugCamera::DebugCamera();
#endif

	// フェードクラスを作成
	CFade::Instance(EFadeType::eVortex);
	// ゲームポーズクラスを作成
	new CGamePause();
	// サウンド管理クラスを作成
	CSoundManager::Instance();
	// BGM管理クラスを作成
	CBGMManager::Instance();
	// 接続部管理クラスを生成
	CConnectPointManager::Instance();
	// 物理管理クラスを生成
	CPhysicsManager::Instance();

#if _DEBUG
	// デバッグモードでは、ブートメニューを最初に開く
	CSceneManager::Instance()->LoadScene(EScene::eBootMenu);
#else
	// リリースモードでは、タイトル画面を最初に開く
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
#endif
}

void CApplication::End()
{
	// ジョイントをすべて削除する
	CPhysicsManager::Instance()->RemoveAllJoint();
	CSceneManager::ClearInstance();
	CTaskManager::ClearInstance();
	CResourceManager::ClearInstance();
	CBGMManager::ClearInstance();
	CSoundManager::ClearInstance();
	CPhysicsManager::ClearInstance();
}

void CApplication::Update()
{
	CTaskManager* taskMgr = CTaskManager::Instance();
	CPhysicsManager* physicsMgr = CPhysicsManager::Instance();

	taskMgr->Delete();
	// 物理の更新処理
	physicsMgr->Update();
	// タスクの更新処理
	taskMgr->Update();
	// 物理の後更新処理
	physicsMgr->LateUpdate();
	// タスクの後更新処理
	taskMgr->LateUpdate();

	// サウンドの更新
	CSoundManager::Instance()->Update();

	// シーンの更新
	CSceneManager::Instance()->Update();

	// タスクの描画処理
	taskMgr->Render();

#if _DEBUG
	// 物理の描画
	physicsMgr->Render();
#endif
}
