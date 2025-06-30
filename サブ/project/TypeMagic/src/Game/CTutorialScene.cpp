#include "CTutorialScene.h"
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
#include "CTutorialMap.h"

//コンストラクタ
CTutorialScene::CTutorialScene()
	: CSceneBase(EScene::eTutorial)
	, mpGameMenu(nullptr)
{
}

//デストラクタ
CTutorialScene::~CTutorialScene()
{
}

//シーン読み込み
void CTutorialScene::Load()
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
	CResourceManager::Load<CModel>("TutorialMap",	"Field\\Map\\TutorialMap\\TutorialMap.obj");
	CResourceManager::Load<CModel>("Wand",			"Wepon\\Wand\\Wand.obj", true);
	CResourceManager::Load<CModel>("Box",			"Field\\Box\\Box.obj");
	CResourceManager::Load<CModel>("Door",			"Field\\Door\\Door.obj");
	CResourceManager::Load<CModel>("SwitchFrame",	"Field\\Switch\\Switch_Frame.obj");
	CResourceManager::Load<CModel>("SwitchButton",	"Field\\Switch\\Switch_Button.obj");
	CResourceManager::Load<CModel>("AirConnectObj", "Field\\AirConnectPoint\\AirConnectPoint.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>("TutorialMap_Ground_Col",	"Field\\Map\\TutorialMap\\Col\\TutorialMap_Ground_Col.obj");
	CResourceManager::Load<CModel>("TutorialMap_Wall_Col",		"Field\\Map\\TutorialMap\\Col\\TutorialMap_Wall_Col.obj");
	CResourceManager::Load<CModel>("Door_Col",					"Field\\Door\\Col\\Door_Col.obj");
	CResourceManager::Load<CModel>("Box_Col",					"Field\\Box\\Col\\Box_Col.obj");
	/*
	効果音
	*/

	// ゲームBGMを読み込み
	// CBGMManager::Instance()->Play(EBGMType::eGame);

	// プレイヤー生成
	CPlayer* player = new CPlayer();


	// CGameCamera2
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 20.0f),
		atPos
	);

	// フィールドクラス生成
	CTutorialMap* field = new CTutorialMap();

	//// 衝突判定するコライダ―を追加
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

//シーンの更新処理
void CTutorialScene::Update()
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
