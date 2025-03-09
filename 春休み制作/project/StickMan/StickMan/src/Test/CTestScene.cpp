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

// プレイヤーの初期座標
#define PLAYER_POS CVector(0.0f,0.0f,50.0f)
// 敵の初期座標
#define ENEMY_POS CVector(0.0f,0.0f,-50.0f)
// 敵の初期方向
#define ENEMY_ROT CVector(0.0f,180.0f,0.0f)

//コンストラクタ
CTestScene::CTestScene()
	: CSceneBase(EScene::eTrashGame)
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
	CResourceManager::Load<CModelX>("TrashPlayer", "Character\\TrashBox\\TrashBoxPlayer.x");
	CResourceManager::Load<CModelX>("TrashEnemy", "Character\\TrashBox\\TrashBoxEnemy.x");
	CResourceManager::Load<CModelX>("Collector", "Character\\Collector\\Fox.x");
	CResourceManager::Load<CModelX>("PunisherCollector", "Character\\PunisherCollector\\PunisherFox.x");
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
	/*
	効果音
	*/
	// プレイヤーと敵の攻撃、車両がプレイヤー、敵、車両にダメージを与えた時の音
	CResourceManager::Load<CSound>("DamageSE", "Sound\\SE\\damage.wav");
	// プレイヤーと敵のクリティカル攻撃がプレイヤー、敵、車両にダメージを与えたときの音
	CResourceManager::Load<CSound>("CriticalSE", "Sound\\SE\\critical.wav");
	// プレイヤーと敵の蓋が閉じている時に攻撃を食らった音
	CResourceManager::Load<CSound>("GuardSE", "Sound\\SE\\guard.wav");
	// プレイヤーの攻撃が回収員に当たった音
	CResourceManager::Load<CSound>("CollectorDamageSE1", "Sound\\SE\\collectorDamage.wav");
	// ゴミを拾った音
	CResourceManager::Load<CSound>("GetSE", "Sound\\SE\\get.wav");
	// 警告音
	CResourceManager::Load<CSound>("AlarmSE", "Sound\\SE\\alarm.wav");
	// メニュー音声
	CResourceManager::Load<CSound>("SelectSE", "Sound\\SE\\MenuSound\\select.wav");
	CResourceManager::Load<CSound>("PushSE", "Sound\\SE\\MenuSound\\push.wav");
	CResourceManager::Load<CSound>("BuzzerSE", "Sound\\SE\\MenuSound\\buzzer.wav");


	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eTrashGame);

	// 経路探索管理クラスを作成
	new CNavManager();




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
	// 衝突判定するコライダ―を追加
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());
	mainCamera->AddCollider(field->GetObjCol());

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
	// 操作説明を設定
	mpGameMenu->SetManual(mpManual);
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

	// メニューポーズでなければ
	if (!CTaskManager::Instance()->IsPaused(PAUSE_MENU_OPEN))
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
}
