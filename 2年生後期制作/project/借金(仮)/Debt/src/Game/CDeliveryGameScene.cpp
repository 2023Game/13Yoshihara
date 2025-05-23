#include "CDeliveryGameScene.h"
#include "CCamera.h"
#include "CGameMenu.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CScoreManager.h"
#include "CResourceManager.h"
#include "CBGMManager.h"
#include "CNavManager.h"
#include "CTimeUI.h"
#include "CDeliveryScoreUI.h"
#include "CInput.h"
#include "Maths.h"
#include "CDeliveryField.h"
#include "CDeliveryFieldManager.h"
#include "CTrashPlayer.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemyManager.h"
#include "CManualMenu.h"
#include "CJobStatusManager.h"

// 制限時間
#define MAX_TIME 100
// リザルトシーンへ行く前の待機時間
#define IDLE_TIME 1.0f

// プレイヤーの初期座標
#define PLAYER_POS CVector(0.0f,10.0f,50.0f)

// 操作説明の画像のパス
#define MANUAL_PATH "UI/delivery_game_manual.png"

//コンストラクタ
CDeliveryGameScene::CDeliveryGameScene()
	: CSceneBase(EScene::eDeliveryGame)
	, mpGameMenu(nullptr)
	, mpTimeUI(nullptr)
	, mElapsedTime(0.0f)
{
}

//デストラクタ
CDeliveryGameScene::~CDeliveryGameScene()
{
}

//シーン読み込み
void CDeliveryGameScene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	// CModelX
	CResourceManager::Load<CModelX>(	"TrashPlayer",		"Character\\TrashBox\\TrashBoxPlayer.x");
	// CModel
	CResourceManager::Load<CModel>(		"DeliveryStage",	"Field\\DeliveryStage\\DeliveryStage.obj");
	CResourceManager::Load<CModel>(		"Sky",				"Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>(		"DeliveryPlayer",	"Character\\DeliveryTruck\\DeliveryTruck_Player.obj");
	CResourceManager::Load<CModel>(		"DeliveryEnemy",	"Character\\DeliveryTruck\\DeliveryTruck_Enemy.obj");
	CResourceManager::Load<CModel>(		"DeliveryItem",		"Field\\Object\\DeliveryItem.obj");
	CResourceManager::Load<CModel>(		"DeliveryHome",		"Field\\Object\\DeliveryHome\\DeliveryHome.obj");
	CResourceManager::Load<CModel>(		"Obstruction",		"Field\\Object\\Obstruction\\Obstruction.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>(		"DeliveryStage_Ground_Collision",	"Field\\DeliveryStage\\CollisionModel\\DeliveryStage_Ground_Collision.obj");
	CResourceManager::Load<CModel>(		"DeliveryHome_Wall_Collision",		"Field\\Object\\DeliveryHome\\CollisionModel\\DeliveryHome_Wall_Collision.obj");
	CResourceManager::Load<CModel>(		"DeliveryHome_Goal_Collision",		"Field\\Object\\DeliveryHome\\CollisionModel\\DeliveryHome_Goal_Collision.obj");
	/*
	効果音
	*/
	// プレイヤーと敵の攻撃、車両がプレイヤー、敵、車両にダメージを与えた時の音
	CResourceManager::Load<CSound>(		"DamageSE",				"Sound\\SE\\damage.wav");
	// プレイヤーと敵のクリティカル攻撃がプレイヤー、敵、車両にダメージを与えたときの音
	CResourceManager::Load<CSound>(		"CriticalSE",			"Sound\\SE\\critical.wav");
	// プレイヤーと敵の蓋が閉じている時に攻撃を食らった音
	CResourceManager::Load<CSound>(		"GuardSE",				"Sound\\SE\\guard.wav");
	// プレイヤーの攻撃が回収員に当たった音
	CResourceManager::Load<CSound>(		"CollectorDamageSE1",	"Sound\\SE\\collectorDamage.wav");
	// アイテムを拾った音
	CResourceManager::Load<CSound>(		"GetSE",				"Sound\\SE\\get.wav");
	// ゴール音
	CResourceManager::Load<CSound>(		"GoalSE",				"Sound\\SE\\goal.wav");
	// 配達物のヒット音
	CResourceManager::Load<CSound>(		"HitSE",				"Sound\\SE\\hit.wav");
	// メニュー音声
	CResourceManager::Load<CSound>(		"SelectSE",				"Sound\\SE\\MenuSound\\select.wav");
	CResourceManager::Load<CSound>(		"PushSE",				"Sound\\SE\\MenuSound\\push.wav");
	CResourceManager::Load<CSound>(		"BuzzerSE",				"Sound\\SE\\MenuSound\\buzzer.wav");


	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eTrashGame);

	// 経路探索管理クラスを作成
	new CNavManager();

	// 配達のフィールド管理クラスを作成
	mpFieldMgr = new CDeliveryFieldManager();

	// プレイヤーの生成
	CDeliveryPlayer* player = new CDeliveryPlayer();
	player->Position(ROAD_LEFT2_POSX, 0.0f, 0.0f);
	player->SetRoadType(ERoadType::eLeft2);

	// 敵の管理クラスを作成
	mpEnemyMgr = new CDeliveryEnemyManager();

	// 時間表示UI生成
	mpTimeUI = new CTimeUI(MAX_TIME);
	// スコア表示UI生成
	mpDeliveryScoreUI = new CDeliveryScoreUI();

	// 操作説明
	mpManual = new CManualMenu(MANUAL_PATH);
	// 初めてのプレイなら操作説明を出す
	auto* jobStatusMgr = CJobStatusManager::Instance();
	if (jobStatusMgr->GetFirstPlay(EJobType::eDelivery))
	{
		mpManual->Open();
		// 次回以降、初めてではなくなる
		jobStatusMgr->SetFirstPlay(EJobType::eDelivery, false);
	}

	// Cameraのテスト
	CVector atPos = CVector(0.0f, 8.0f, 0.0f);
	CCamera* mainCamera = new CCamera
	(
		atPos + CVector(0.0f, 400.0f, 200.0f),
		atPos
	);
	 
	// CGameCamera2のテスト
	//CVector atPos = player->Position() + CVector(0.0f, 8.0f, 0.0f);
	//CGameCamera2* mainCamera = new CGameCamera2
	//(
	//	atPos + CVector(0.0f, 10.0f, 400.0f),
	//	atPos
	//);
	//bool end = false;
	//// 全てのフィールドのコライダーを追加するまでループ
	//for (int i = 0; i >= 0; i++)
	//{
	//	CDeliveryField* field = mpFieldMgr->GetField(i, end);
	//	// 衝突判定するコライダ―を追加
	//	mainCamera->AddCollider(field->GetGroundCol());
	//	mainCamera->AddCollider(field->GetWallCol());
	//	mainCamera->AddCollider(field->GetObjCol());

	//	// 最後の要素のコライダ―まで追加したらループ終了
	//	if (end) break;
	//}

	//mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
	// 操作説明を設定
	mpGameMenu->SetManual(mpManual);
}

//シーンの更新処理
void CDeliveryGameScene::Update()
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
		// 敵管理クラスの更新
		mpEnemyMgr->Update();
		// フィールド管理クラスの更新
		mpFieldMgr->Update();
		// 時間表示UIクラスの更新
		mpTimeUI->Update();
		// スコア表示UIクラスの更新
		mpDeliveryScoreUI->Update();
	}

	// プレイヤークラスを取得
	CDeliveryPlayer* player = dynamic_cast<CDeliveryPlayer*>(CDeliveryPlayer::Instance());
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
			
			int accuracyInt = 0;
			float accuracy = 0.0f;
			// 1発も打っていない場合は処理しない
			if (!player->GetShotNum() == 0)
			{
				// 命中率を求める
				// 割合計算後に小数点表記から%表記に変換して小数点以下2桁より後を切り捨てる
				accuracyInt = Math::Clamp01((float)player->GetHitNum() / player->GetShotNum()) * 100;
				// 小数点表記に戻す
				accuracy = accuracyInt / 100.0f;
			}
			
			// スコアデータを設定
			scoreMgr->SetDeliveryGameScoreData(mpDeliveryScoreUI->GetScore(),
				player->GetDeliveryNum(), player->GetDestroyEnemyNum(),
				accuracy);
			// ゲームの種類を今のシーンに設定
			scoreMgr->SetGameType((int)sceneMgr->GetCurrentScene());
			// リザルトシーンへ移行
			sceneMgr->LoadScene(EScene::eResult);
		}
	}
}
