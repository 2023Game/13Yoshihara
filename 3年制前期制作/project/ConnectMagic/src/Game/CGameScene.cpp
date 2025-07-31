#include "CGameScene.h"
#include "CResourceManager.h"
#include "CBGMManager.h"
#include "CConnectPointManager.h"
#include "CRoomManager.h"
#include "CGameCamera2.h"
#include "CGameMenu.h"
#include "CInput.h"
#include "CPlayer.h"
#include "CRoomBase.h"

// プレイヤーのスタート地点
#define PLAYER_POS		CVector(0.0f,0.0f,-30.0f)

// コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
{
}

// デストラクタ
CGameScene::~CGameScene()
{
}

// シーン読み込み
void CGameScene::Load()
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
	CResourceManager::Load<CModel>("Wand",			"Wepon\\Wand\\Wand.obj", true);
	CResourceManager::Load<CModel>("Box",			"Field\\Box\\Box.obj");
	CResourceManager::Load<CModel>("SwitchFrame",	"Field\\Switch\\Switch_Frame.obj");
	CResourceManager::Load<CModel>("SwitchButton",	"Field\\Switch\\Switch_Button.obj");
	CResourceManager::Load<CModel>("ConnectRoom",	"Field\\Room\\ConnectRoom\\ConnectRoom.obj");
	CResourceManager::Load<CModel>("Room1",			"Field\\Room\\Room1\\Room1.obj");
	CResourceManager::Load<CModel>("Room2",			"Field\\Room\\Room2\\Room2.obj");
	CResourceManager::Load<CModel>("Room3",			"Field\\Room\\Room3\\Room3.obj");
	CResourceManager::Load<CModel>("Room4",			"Field\\Room\\Room4\\Room4.obj");
	CResourceManager::Load<CModel>("Room5",			"Field\\Room\\Room5\\Room5.obj");
	CResourceManager::Load<CModel>("LastRoom",		"Field\\Room\\LastRoom\\LastRoom.obj");
	CResourceManager::Load<CModel>("MoveObject",	"Field\\MoveObject\\MoveObject.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>("Box_Col",					"Field\\Box\\Col\\Box_Col.obj");
	CResourceManager::Load<CModel>("ConnectRoom_Ground_Col",	"Field\\Room\\ConnectRoom\\Col\\ConnectRoom_Ground_Col.obj");
	CResourceManager::Load<CModel>("ConnectRoom_Wall_Col",		"Field\\Room\\ConnectRoom\\Col\\ConnectRoom_Wall_Col.obj");
	CResourceManager::Load<CModel>("Room1_Ground_Col",			"Field\\Room\\Room1\\Col\\Room1_Ground_Col.obj");
	CResourceManager::Load<CModel>("Room1_Wall_Col",			"Field\\Room\\Room1\\Col\\Room1_Wall_Col.obj");
	CResourceManager::Load<CModel>("Room2_Ground_Col",			"Field\\Room\\Room2\\Col\\Room2_Ground_Col.obj");
	CResourceManager::Load<CModel>("Room2_Wall_Col",			"Field\\Room\\Room2\\Col\\Room2_Wall_Col.obj");
	CResourceManager::Load<CModel>("Room3_Ground_Col",			"Field\\Room\\Room3\\Col\\Room3_Ground_Col.obj");
	CResourceManager::Load<CModel>("Room3_Wall_Col",			"Field\\Room\\Room3\\Col\\Room3_Wall_Col.obj");
	CResourceManager::Load<CModel>("Room4_Ground_Col",			"Field\\Room\\Room4\\Col\\Room4_Ground_Col.obj");
	CResourceManager::Load<CModel>("Room4_Wall_Col",			"Field\\Room\\Room4\\Col\\Room4_Wall_Col.obj");
	CResourceManager::Load<CModel>("Room5_Ground_Col",			"Field\\Room\\Room5\\Col\\Room5_Ground_Col.obj");
	CResourceManager::Load<CModel>("Room5_Wall_Col",			"Field\\Room\\Room5\\Col\\Room5_Wall_Col.obj");
	CResourceManager::Load<CModel>("LastRoom_Ground_Col",		"Field\\Room\\LastRoom\\Col\\LastRoom_Ground_Col.obj");
	CResourceManager::Load<CModel>("LastRoom_Wall_Col",			"Field\\Room\\LastRoom\\Col\\LastRoom_Wall_Col.obj");
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
	player->Position(PLAYER_POS);

	// CGameCamera2のテスト
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 20.0f),
		atPos
	);
	// カメラの位置を反対へ
	//mainCamera->SetRotateAngle(CVector(0.0f, 180.0f, 0.0f));

	// 部屋管理クラスを生成
	mpRoomMgr = new CRoomManager();

	std::vector<CRoomBase*> rooms = mpRoomMgr->GetRooms();
	for (int i = 0; i < rooms.size(); i++)
	{
		// 衝突判定するコライダーを追加
		pointMgr->AddCollider(rooms[i]->GetGroundCol());
		pointMgr->AddCollider(rooms[i]->GetWallCol());

		// 衝突判定するコライダ―を追加
		mainCamera->AddCollider(rooms[i]->GetGroundCol());
		mainCamera->AddCollider(rooms[i]->GetWallCol());
		mainCamera->AddCollider(rooms[i]->GetObjCol());
	}

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

// シーンの更新処理
void CGameScene::Update()
{
}
