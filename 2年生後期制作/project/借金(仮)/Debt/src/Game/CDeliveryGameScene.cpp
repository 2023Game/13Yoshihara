#include "CDeliveryGameScene.h"
#include "CGameCamera2.h"
#include "CGameMenu.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CScoreManager.h"
#include "CResourceManager.h"
#include "CBGMManager.h"
#include "CNavManager.h"
#include "CTimeUI.h"
#include "CTrashScoreUI.h"
#include "CInput.h"
#include "Maths.h"
#include "CTrashField.h"
#include "CTrashPlayer.h"
#include "CDeliveryPlayer.h"
#include "CDeliveryEnemy.h"

// 制限時間
#define MAX_TIME 100
// リザルトシーンへ行く前の待機時間
#define IDLE_TIME 1.0f

// プレイヤーの初期座標
#define PLAYER_POS CVector(0.0f,0.0f,50.0f)

//コンストラクタ
CDeliveryGameScene::CDeliveryGameScene()
	: CSceneBase(EScene::eDeliveryGame)
	, mpGameMenu(nullptr)
	, mpTimeUI(nullptr)
	, mpTrashScoreUI(nullptr)
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
	CResourceManager::Load<CModelX>(	"TrashPlayer", "Character\\TrashBox\\TrashBoxPlayer.x");
	// CModel
	CResourceManager::Load<CModel>(		"TrashStage",		"Field\\TrashStage\\TrashStage.obj");
	CResourceManager::Load<CModel>(		"Sky",				"Field\\Sky\\Sky.obj");
	CResourceManager::Load<CModel>(		"DeliveryPlayer",	"Character\\DeliveryTruck\\DeliveryTruck_Player.obj");
	CResourceManager::Load<CModel>(		"DeliveryEnemy",	"Character\\DeliveryTruck\\DeliveryTruck_Enemy.obj");
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
	// メニュー音声
	CResourceManager::Load<CSound>("SelectSE", "Sound\\SE\\MenuSound\\select.wav");
	CResourceManager::Load<CSound>("PushSE", "Sound\\SE\\MenuSound\\push.wav");
	CResourceManager::Load<CSound>("BuzzerSE", "Sound\\SE\\MenuSound\\buzzer.wav");


	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eTrashGame);

	// 経路探索管理クラスを作成
	new CNavManager();

	// ゴミ拾いのフィールドクラスを作成
	CTrashField* field = new CTrashField();

	CTrashPlayer* player = new CTrashPlayer();
	player->Position(PLAYER_POS);

	CDeliveryPlayer* deliveryPlayer = new CDeliveryPlayer();
	deliveryPlayer->Position(-20.0f, 0.0f, 0.0f);
	CDeliveryEnemy* enemy = new CDeliveryEnemy();
	enemy->Position(20.0f, 0.0f, 0.0f);

	// 時間表示UI生成
	mpTimeUI = new CTimeUI(MAX_TIME);
	// スコア表示UI生成
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
}

//シーンの更新処理
void CDeliveryGameScene::Update()
{
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

	// 時間表示UIクラスの更新
	mpTimeUI->Update();
	// スコア表示UIクラスの更新
	mpTrashScoreUI->Update();
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
				player->GetTrashBag(), player->GetGoldTrashBag(), hpPer);
			// ゲームの種類を今のシーンに設定
			scoreMgr->SetGameType((int)sceneMgr->GetCurrentScene());
			// リザルトシーンへ移行
			sceneMgr->LoadScene(EScene::eResult);
		}
	}
}
