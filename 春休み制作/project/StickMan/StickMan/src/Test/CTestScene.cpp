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
#include "CFieldManager.h"
#include "CClassRoom.h"
#include "CTable.h"

#include "CMap_1.h"
#include "CMap_2.h"

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
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\Player.x");
	// CModel
	CResourceManager::Load<CModel>("TestField", "Field\\Field.obj");
	CResourceManager::Load<CModel>("ClassRoom", "Field\\Map\\ClassRoom.obj");
	CResourceManager::Load<CModel>("Table",		"Field\\Map\\SchoolTable.obj");
	CResourceManager::Load<CModel>("Book",		"Field\\Map\\Book.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>("TableCol",	"Field\\Map\\Col\\SchoolTableCol.obj");
	/*
	効果音
	*/
	


	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// 経路探索管理クラスを作成
	new CNavManager();

	// プレイヤー生成
	CPlayer* player = new CPlayer();

	// 教室の生成
	CClassRoom* classRoom = new CClassRoom();
	// 机の生成
	CTable* table = new CTable();
	// 教室の位置を調整
	classRoom->Position(0.0f, -110.0f * table->Scale().Y(), 0.0f);


	// フィールド管理クラス生成
	/*CFieldManager* fieldMgr = new CFieldManager();*/




	// CGameCameraのテスト
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);
	// 
	// CGameCamera2のテスト
	CVector atPos = player->Position() + CVector(0.0f, 8.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 10.0f, 50.0f),
		atPos
	);
	//// 衝突判定するコライダ―を追加
	//mainCamera->AddCollider(fieldMgr->GetGroundCol());
	//mainCamera->AddCollider(fieldMgr->GetWallCol());
	//mainCamera->AddCollider(fieldMgr->GetObjCol());

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
