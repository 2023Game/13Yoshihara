#include "CHomeScene.h"
#include "CSceneManager.h"
#include "CHomeField.h"
#include "CHomePlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CToDoUI.h"
#include "CSelectJobUI.h"
#include "CJobStatusManager.h"
#include "JobType.h"
#include "CMoneyUI.h"
#include "CDebtMoneyUI.h"
#include "CTaskManager.h"

// やることのテキスト
#define TODO_TEXT0 "PCから仕事を選択する"
#define TODO_TEXT1 "ドアから仕事へ行く"

//コンストラクタ
CHomeScene::CHomeScene()
	: CSceneBase(EScene::eHome)
	, mpGameMenu(nullptr)
{
}

//デストラクタ
CHomeScene::~CHomeScene()
{
}

//シーン読み込み
void CHomeScene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う
	CResourceManager::Load<CModelX>(	"Player",	"Character\\Player\\player.x");

	// 拠点
	CResourceManager::Load<CModel>(		"HomeBase",								"Field\\HomeBase\\HomeBase.obj");
	CResourceManager::Load<CModel>(		"Bed",									"Field\\Object\\Bed.obj");
	CResourceManager::Load<CModel>(		"Chair",								"Field\\Object\\Chair.obj");
	CResourceManager::Load<CModel>(		"Computer",								"Field\\Object\\Computer.obj");
	CResourceManager::Load<CModel>(		"Desk",									"Field\\Object\\Desk.obj");
	CResourceManager::Load<CModel>(		"Door",									"Field\\Object\\Door.obj");
	CResourceManager::Load<CModel>(		"Rack",									"Field\\Object\\Rack.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>(		"HomeBase_Ground&Ceiling_Collision",	"Field\\HomeBase\\CollisionModel\\HomeBase_Ground&Ceiling_Collision.obj");
	CResourceManager::Load<CModel>(		"HomeBase_Wall_Collision",				"Field\\HomeBase\\CollisionModel\\HomeBase_Wall_Collision.obj");
	CResourceManager::Load<CModel>(		"HomeBase_Object_Collision",			"Field\\HomeBase\\CollisionModel\\HomeBase_Object_Collision.obj");
	/*
	効果音
	*/
	CResourceManager::Load<CSound>(		"SelectSE",		"Sound\\SE\\MenuSound\\select.wav");
	CResourceManager::Load<CSound>(		"PushSE",		"Sound\\SE\\MenuSound\\push.wav");
	CResourceManager::Load<CSound>(		"BuzzerSE",		"Sound\\SE\\MenuSound\\buzzer.wav");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eHome);

	CHomeField* field = new CHomeField();

	CHomePlayer* player = new CHomePlayer();
	player->Scale(1.0f, 1.0f, 1.0f);

	// やること表示UIを生成
	mpToDoUI = new CToDoUI();
	mpToDoUI->AddText(TODO_TEXT0);
	mpToDoUI->AddText(TODO_TEXT1);

	// 選択中の仕事名表示UIを生成
	mpSelectJobUI = new CSelectJobUI();

	// 所持金UI
	mpMoneyUI = new CMoneyUI();

	// 返済額UI
	CDebtMoneyUI* debtMoneyUI = new CDebtMoneyUI();

	// CGameCameraのテスト
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CGameCamera2のテスト
	CVector atPos = player->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 10.0f, 15.0f),
		atPos
	);
	// 衝突判定するコライダ―を追加
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());
	mainCamera->AddCollider(field->GetObjCol());

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CMenu();
}

//シーンの更新処理
void CHomeScene::Update()
{
#if _DEBUG
	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
#endif

	// メニューポーズでなければ
	if(!CTaskManager::Instance()->IsPaused(PAUSE_MENU_OPEN))
	{
		// ゲームメニューを開いてなければ、[TAB]キーでメニューを開く
		if (!mpGameMenu->IsOpened())
		{
			if (CInput::PushKey(VK_TAB))
			{
				mpGameMenu->Open();
			}
		}
	}
	EJobType jobType = CJobStatusManager::Instance()->GetSelectJob();
	// 仕事が選択されていない場合
	if (jobType == EJobType::eNone)
	{
		mpToDoUI->SetText(0);
	}
	// 仕事が選択されている場合
	else
	{
		mpToDoUI->SetText(1);
	}
	// 選択中の仕事名表示UIの更新
	mpSelectJobUI->Update();
	// 所持金UIの更新
	mpMoneyUI->Update();
}
