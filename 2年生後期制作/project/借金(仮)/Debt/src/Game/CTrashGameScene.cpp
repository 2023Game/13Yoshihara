#include "CTrashGameScene.h"
#include "CSceneManager.h"
#include "CTrashField.h"
#include "CTrashPlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CVehicleManager.h"
#include "CResidentManager.h"
#include "CTrashEnemy.h"
#include "CNavManager.h"
#include "CTimeUI.h"
#include "CTrashScoreUI.h"

// 制限時間
#define MAX_TIME 100

//コンストラクタ
CTrashGameScene::CTrashGameScene()
	: CSceneBase(EScene::eTrashGame)
	, mpGameMenu(nullptr)
	, mpVehicleManager(nullptr)
	, mpResidentManager(nullptr)
{
}

//デストラクタ
CTrashGameScene::~CTrashGameScene()
{
}

//シーン読み込み
void CTrashGameScene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う
	
	// CModelX
	CResourceManager::Load<CModelX>("TrashPlayer", "Character\\TrashBox\\TrashBoxPlayer.x");
	CResourceManager::Load<CModelX>("TrashEnemy", "Character\\TrashBox\\TrashBoxEnemy.x");
	CResourceManager::Load<CModelX>("Collector", "Character\\Collector\\Fox.x");
	CResourceManager::Load<CModelX>("Resident", "Character\\Resident\\Monkey.x");
	// CModel
	CResourceManager::Load<CModel>("TrashStage", "Field\\TrashStage\\TrashStage.obj");
	CResourceManager::Load<CModel>("Sky", "Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>("TrashBox", "Field\\Object\\TrashBox.obj");
	CResourceManager::Load<CModel>("Car", "Character\\Car\\Car.obj");
	CResourceManager::Load<CModel>("GarbageTruck", "Character\\GarbageTruck\\GarbageTruck.obj");
	CResourceManager::Load<CModel>("BlackTruck", "Character\\BlackTruck\\BlackTruck.obj");
	CResourceManager::Load<CModel>("TrashBag", "Field\\Object\\TrashBag.obj");
	CResourceManager::Load<CModel>("TrashBagGold", "Field\\Object\\TrashBagGold.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>("TrashStage_Ground_Collision", "Field\\TrashStage\\CollisionModel\\TrashStage_Ground_Collision.obj");
	CResourceManager::Load<CModel>("TrashStage_Wall_Collision", "Field\\TrashStage\\CollisionModel\\TrashStage_Wall_Collision.obj");
	CResourceManager::Load<CModel>("TrashStage_Object_Collision", "Field\\TrashStage\\CollisionModel\\TrashStage_Object_Collision.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eHome);

	// 経路探索管理クラスを作成
	new CNavManager();

	// ゴミ拾いのフィールドクラスを作成
	CTrashField* field = new CTrashField();

	// 車両の管理クラス作成
	mpVehicleManager =
		new CVehicleManager();

	// 住人の管理クラス作成
	mpResidentManager =
		new CResidentManager();

	CTrashPlayer* player = new CTrashPlayer();

	float enemyScale = 0.1f;
	CTrashEnemy* enemy = new CTrashEnemy(false, enemyScale);
	enemy->Position(0.0f, 0.0f, 5.0f);

	// 時間表示UI作成
	mpTimeUI = new CTimeUI(MAX_TIME);
	// スコア表示UI作成
	mpTrashScoreUI = new CTrashScoreUI();

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
		atPos + CVector(0.0f, 0.0f, 50.0f),
		atPos
	);
	// 衝突判定するコライダ―を追加
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());
	mainCamera->AddCollider(field->GetObjCol());

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

//シーンの更新処理
void CTrashGameScene::Update()
{
	// BGM再生中でなければ、BGMを再生
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}

	// 車両管理クラスの更新
	mpVehicleManager->Update();
	// 時間表示UIクラスの更新
	mpTimeUI->Update();
	// スコア表示UIクラスの更新
	mpTrashScoreUI->Update();
}
