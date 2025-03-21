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
#include "CTrashVehicleManager.h"
#include "CResidentManager.h"
#include "CNavManager.h"
#include "CTimeUI.h"
#include "CTrashScoreUI.h"
#include "CScoreManager.h"
#include "CTaskManager.h"
#include "CTrashEnemyManager.h"
#include "CTrashWarningUI.h"
#include "Maths.h"
#include "CManualMenu.h"
#include "CJobStatusManager.h"

// 制限時間
#define MAX_TIME 100
// リザルトシーンへ行く前の待機時間
#define IDLE_TIME 1.0f

// プレイヤーの初期座標
#define PLAYER_POS CVector(0.0f,0.0f,50.0f)
// 敵の初期座標
#define ENEMY_POS CVector(0.0f,0.0f,-50.0f)
// 敵の初期方向
#define ENEMY_ROT CVector(0.0f,180.0f,0.0f)

// 操作説明の画像のパス
#define MANUAL_PATH "UI/trash_game_manual.png"

//コンストラクタ
CTrashGameScene::CTrashGameScene()
	: CSceneBase(EScene::eTrashGame)
	, mpGameMenu(nullptr)
	, mpVehicleMgr(nullptr)
	, mpResidentMgr(nullptr)
	, mpTrashEnemyMgr(nullptr)
	, mpTimeUI(nullptr)
	, mpTrashScoreUI(nullptr)
	, mElapsedTime(0.0f)
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
	CResourceManager::Load<CModelX>(	"TrashPlayer",					"Character\\TrashBox\\TrashBoxPlayer.x");
	CResourceManager::Load<CModelX>(	"TrashEnemy",					"Character\\TrashBox\\TrashBoxEnemy.x");
	CResourceManager::Load<CModelX>(	"Collector",					"Character\\Collector\\Fox.x");
	CResourceManager::Load<CModelX>(	"PunisherCollector",			"Character\\PunisherCollector\\PunisherFox.x");
	CResourceManager::Load<CModelX>(	"Resident",						"Character\\Resident\\Monkey.x");
	// CModel
	CResourceManager::Load<CModel>(		"TrashStage",					"Field\\TrashStage\\TrashStage.obj");
	CResourceManager::Load<CModel>(		"Sky",							"Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>(		"TrashBox",						"Field\\Object\\TrashBox.obj");
	CResourceManager::Load<CModel>(		"Car",							"Character\\Car\\Car.obj");
	CResourceManager::Load<CModel>(		"GarbageTruck",					"Character\\GarbageTruck\\GarbageTruck.obj");
	CResourceManager::Load<CModel>(		"BlackTruck",					"Character\\BlackTruck\\BlackTruck.obj");
	CResourceManager::Load<CModel>(		"TrashBag",						"Field\\Object\\TrashBag.obj");
	CResourceManager::Load<CModel>(		"TrashBagGold",					"Field\\Object\\TrashBagGold.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>(		"TrashStage_Ground_Collision",	"Field\\TrashStage\\CollisionModel\\TrashStage_Ground_Collision.obj");
	CResourceManager::Load<CModel>(		"TrashStage_Wall_Collision",	"Field\\TrashStage\\CollisionModel\\TrashStage_Wall_Collision.obj");
	CResourceManager::Load<CModel>(		"TrashStage_Object_Collision",	"Field\\TrashStage\\CollisionModel\\TrashStage_Object_Collision.obj");
	/*
	効果音
	*/
	// プレイヤーと敵の攻撃、車両がプレイヤー、敵、車両にダメージを与えた時の音
	CResourceManager::Load<CSound>(		"DamageSE",						"Sound\\SE\\damage.wav");
	// プレイヤーと敵のクリティカル攻撃がプレイヤー、敵、車両にダメージを与えたときの音
	CResourceManager::Load<CSound>(		"CriticalSE",					"Sound\\SE\\critical.wav");
	// プレイヤーと敵の蓋が閉じている時に攻撃を食らった音
	CResourceManager::Load<CSound>(		"GuardSE",						"Sound\\SE\\guard.wav");
	// プレイヤーの攻撃が回収員に当たった音
	CResourceManager::Load<CSound>(		"CollectorDamageSE1",			"Sound\\SE\\collectorDamage.wav");
	// ゴミを拾った音
	CResourceManager::Load<CSound>(		"GetSE",						"Sound\\SE\\get.wav");
	// 警告音
	CResourceManager::Load<CSound>(		"AlarmSE",						"Sound\\SE\\alarm.wav");
	// メニュー音声
	CResourceManager::Load<CSound>(		"SelectSE",						"Sound\\SE\\MenuSound\\select.wav");
	CResourceManager::Load<CSound>(		"PushSE",						"Sound\\SE\\MenuSound\\push.wav");
	CResourceManager::Load<CSound>(		"BuzzerSE",						"Sound\\SE\\MenuSound\\buzzer.wav");


	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eTrashGame);

	// 経路探索管理クラスを作成
	new CNavManager();

	// ゴミ拾いのフィールドクラスを作成
	CTrashField* field = new CTrashField();

	// 車両の管理クラス作成
	mpVehicleMgr =
		new CTrashVehicleManager();

	// 住人の管理クラス作成
	mpResidentMgr =
		new CResidentManager();

	CTrashPlayer* player = new CTrashPlayer();
	player->Position(PLAYER_POS);

	// ゴミ拾いの敵管理クラス作成
	mpTrashEnemyMgr =
		new CTrashEnemyManager();

	// 時間表示UI生成
	mpTimeUI = new CTimeUI(MAX_TIME);
	// スコア表示UI生成
	mpTrashScoreUI = new CTrashScoreUI();
	// 警告表示用UI生成
	mpWarningUI = new CTrashWarningUI();

	// 操作説明
	mpManual = new CManualMenu(MANUAL_PATH);
	// 初めてのプレイなら操作説明を出す
	auto* jobStatusMgr = CJobStatusManager::Instance();
	if (jobStatusMgr->GetFirstPlay(EJobType::eTrash))
	{
		mpManual->Open();
		// 次回以降、初めてではなくなる
		jobStatusMgr->SetFirstPlay(EJobType::eTrash, false);
	}

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
	mpVehicleMgr->SetCameraCollision(mainCamera);

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
	// 操作説明を設定
	mpGameMenu->SetManual(mpManual);
}

//シーンの更新処理
void CTrashGameScene::Update()
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
				mpWarningUI->StopSE();
			}
		}
		// 車両管理クラスの更新
		mpVehicleMgr->Update();
		// 敵管理クラスの更新
		mpTrashEnemyMgr->Update();
		// 時間表示UIクラスの更新
		mpTimeUI->Update();
		// スコア表示UIクラスの更新
		mpTrashScoreUI->Update();
	}

	// プレイヤークラスを取得
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
	// 制限時間が0になったか、
	// プレイヤーの死亡によってゲームを終了する時
	if (mpTimeUI->GetTime() < 0 ||
		player->GetGameEnd())
	{
		// ゲームをポーズ
		CTaskManager::Instance()->Pause(PAUSE_GAME);
		// 待機時間が経過したらリザルトシーンへ
		mElapsedTime += Times::DeltaTime();
		if (mElapsedTime >= IDLE_TIME)
		{
			// 得点管理クラスを取得
			CScoreManager* scoreMgr = CScoreManager::Instance();
			// シーン管理クラスを取得
			CSceneManager* sceneMgr = CSceneManager::Instance();

			// 残りHPの割合を求める
			float hpPer = Math::Clamp01((float)player->GetHp() / player->GetMaxHp());
			// スコアデータを設定
			scoreMgr->SetTrashGameScoreData(mpTrashScoreUI->GetScore(),
				player->GetTrashBag(), player->GetGoldTrashBag(),hpPer);
			// ゲームの種類を今のシーンに設定
			scoreMgr->SetGameType((int)sceneMgr->GetCurrentScene());
			// リザルトシーンへ移行
			sceneMgr->LoadScene(EScene::eResult);
		}
	}
}
