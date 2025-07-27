#include "CMap1Scene.h"
#include "CSceneManager.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"
#include "CTaskManager.h"
#include "Maths.h"
#include "CResourceManager.h"
#include "CPlayer.h"
#include "CWand.h"
#include "CMap1.h"
#include "CConnectPointManager.h"

// コンストラクタ
CMap1Scene::CMap1Scene()
	: CSceneBase(EScene::eMap1)
	, mpGameMenu(nullptr)
{
}

// デストラクタ
CMap1Scene::~CMap1Scene()
{
}

// シーン読み込み
void CMap1Scene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う

	// CModelX
	CResourceManager::Load<CModelX>("Player", "Character\\Adventurer\\Adventurer.x", true);
	// CModel
	CResourceManager::Load<CModel>("Map1",			"Field\\Map\\Map1\\Map1.obj");
	CResourceManager::Load<CModel>("Wand",			"Wepon\\Wand\\Wand.obj", true);
	CResourceManager::Load<CModel>("Box",			"Field\\Box\\Box.obj");
	CResourceManager::Load<CModel>("Door",			"Field\\Door\\Door.obj");
	CResourceManager::Load<CModel>("SwitchFrame",	"Field\\Switch\\Switch_Frame.obj");
	CResourceManager::Load<CModel>("SwitchButton",	"Field\\Switch\\Switch_Button.obj");
	CResourceManager::Load<CModel>("AirConnectObj", "Field\\AirConnectPoint\\AirConnectPoint.obj");
	CResourceManager::Load<CModel>("Torch",			"Field\\Torch\\Torch.obj");
	CResourceManager::Load<CModel>("Bridge",		"Field\\Bridge\\Bridge.obj");
	CResourceManager::Load<CModel>("CampFire",		"Field\\CampFire\\CampFire.obj");
	CResourceManager::Load<CModel>("DownWall",		"Field\\DownWall\\DownWall.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>("Map1_Ground_Col",	"Field\\Map\\Map1\\Col\\Map1_Ground_Col.obj");
	CResourceManager::Load<CModel>("Map1_Wall_Col",		"Field\\Map\\Map1\\Col\\Map1_Wall_Col.obj");
	CResourceManager::Load<CModel>("Door_Col",			"Field\\Door\\Col\\Door_Col.obj");
	CResourceManager::Load<CModel>("Box_Col",			"Field\\Box\\Col\\Box_Col.obj");
	CResourceManager::Load<CModel>("Bridge_Col",		"Field\\Bridge\\Col\\Bridge_Col.obj");
	CResourceManager::Load<CModel>("CampFire_Col",		"Field\\CampFire\\Col\\CampFire_Col.obj");
	/*
	効果音
	*/

	// 接続部管理クラスを取得
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// 衝突判定をするコライダーをリセット
	pointMgr->ResetCollider();

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// プレイヤー生成
	CPlayer* player = new CPlayer();

	// CGameCamera2のテスト
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 20.0f),
		atPos
	);
	// カメラの位置を反対へ
	mainCamera->SetRotateAngle(CVector(0.0f, 180.0f, 0.0f));

	// フィールドクラス生成
	mpField = new CMap1();

	// 衝突判定するコライダーを追加
	pointMgr->AddCollider(mpField->GetGroundCol());
	pointMgr->AddCollider(mpField->GetWallCol());

	// 衝突判定するコライダ―を追加
	mainCamera->AddCollider(mpField->GetGroundCol());
	mainCamera->AddCollider(mpField->GetWallCol());
	mainCamera->AddCollider(mpField->GetObjCol());

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

// シーンの更新処理
void CMap1Scene::Update()
{
#if _DEBUG
	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
#endif

	// ゲームメニューを開いてなければ、[TAB]キーでメニューを開く
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey(VK_TAB))
		{
			mpGameMenu->Open();
		}
	}
	// ゲームメニューを開いていれば、[TAB]キーでメニューを閉じる
	else
	{
		if (CInput::PushKey(VK_TAB))
		{
			mpGameMenu->Close();
		}
	}
}
