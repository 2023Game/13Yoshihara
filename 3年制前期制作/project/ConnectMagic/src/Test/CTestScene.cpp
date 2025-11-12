#include "CTestScene.h"
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
#include "CTestField.h"
#include "CConnectPointManager.h"

//コンストラクタ
CTestScene::CTestScene()
	: CSceneBase(EScene::eGameTest)
	, mpGameMenu(nullptr)
{
}

//デストラクタ
CTestScene::~CTestScene()
{
}

//シーン読み込み
void CTestScene::Load()
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
	CResourceManager::Load<CModel>("TestField",		"Field\\field.obj");
	CResourceManager::Load<CModel>("Wand",			"Wepon\\Wand\\Wand.obj", true);
	CResourceManager::Load<CModel>("Weight",		"Field\\Weight\\Weight.obj");
	CResourceManager::Load<CModel>("Door",			"Field\\Door\\Door.obj");
	CResourceManager::Load<CModel>("SwitchFrame",	"Field\\Switch\\Switch_Frame.obj");
	CResourceManager::Load<CModel>("SwitchButton",	"Field\\Switch\\Switch_Button.obj");
	CResourceManager::Load<CModel>("AirConnectObj",	"Field\\AirConnectPoint\\AirConnectPoint.obj");
	CResourceManager::Load<CModel>("Torch",			"Field\\Torch\\Torch.obj");
	CResourceManager::Load<CModel>("Bridge",		"Field\\Bridge\\Bridge.obj");
	CResourceManager::Load<CModel>("CampFire",		"Field\\CampFire\\CampFire.obj");
	CResourceManager::Load<CModel>("DownWall",		"Field\\DownWall\\DownWall.obj");
	CResourceManager::Load<CModel>("ConnectRoom",	"Field\\Room\\ConnectRoom\\ConnectRoom.obj");
	CResourceManager::Load<CModel>("Room1",			"Field\\Room\\Room1\\Room1.obj");
	CResourceManager::Load<CModel>("MoveObject",	"Field\\MoveObject\\MoveObject.obj");
	CResourceManager::Load<CModel>("CrystalOn",		"Field\\Crystal\\CrystalOn.obj");
	CResourceManager::Load<CModel>("CrystalOff",	"Field\\Crystal\\CrystalOff.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>("TestField_Ground_Col",	"Field\\Map\\Map_1\\Col\\Map_1_Ground_Col.obj");
	CResourceManager::Load<CModel>("TestField_Wall_Col",	"Field\\Map\\Map_1\\Col\\Map_1_Wall_Col.obj");
	CResourceManager::Load<CModel>("Door_Col",				"Field\\Door\\Col\\Door_Col.obj");
	CResourceManager::Load<CModel>("weight_Col",			"Field\\weight\\Col\\weight_Col.obj");
	CResourceManager::Load<CModel>("Bridge_Col",			"Field\\Bridge\\Col\\Bridge_Col.obj");
	CResourceManager::Load<CModel>("CampFire_Col",			"Field\\CampFire\\Col\\CampFire_Col.obj");
	CResourceManager::Load<CModel>("ConnectRoom_Ground_Col","Field\\Room\\ConnectRoom\\Col\\ConnectRoom_Ground_Col.obj");
	CResourceManager::Load<CModel>("ConnectRoom_Wall_Col",	"Field\\Room\\ConnectRoom\\Col\\ConnectRoom_Wall_Col.obj");
	CResourceManager::Load<CModel>("Room1_Ground_Col",		"Field\\Room\\Room1\\Col\\Room1_Ground_Col.obj");
	CResourceManager::Load<CModel>("Room1_Wall_Col",		"Field\\Room\\Room1\\Col\\Room1_Wall_Col.obj");
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

	// CGameCameraのテスト
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);
	// 
	// CGameCamera2のテスト
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 20.0f),
		atPos
	);

	// フィールドクラス生成
	CTestField* field = new CTestField();

	// 衝突判定するコライダーを追加
	pointMgr->AddCollider(field->GetGroundCol());
	pointMgr->AddCollider(field->GetWallCol());

	//// 衝突判定するコライダ―を追加
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

//シーンの更新処理
void CTestScene::Update()
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
