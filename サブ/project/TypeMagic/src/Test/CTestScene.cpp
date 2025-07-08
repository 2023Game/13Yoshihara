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
#include "CTestField.h"
#include "CEnemy.h"
#include "CEnemyManager.h"

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
	CResourceManager::Load<CModel>("TestField",			"Field\\field.obj");
	CResourceManager::Load<CModel>("TestModel",			"Character\\TestModel.obj");
	// 炎属性
	CResourceManager::Load<CModel>("FireBall",			"Wepon\\Spell_Attack\\FireBall.obj");
	CResourceManager::Load<CModel>("FireBolt",			"Wepon\\Spell_Attack\\FireBolt.obj");
	CResourceManager::Load<CModel>("FireBreath",		"Wepon\\Spell_Attack\\FireBreath.obj");
	CResourceManager::Load<CModel>("FireTeleport",		"Wepon\\Spell_Support\\FireTeleport.obj");
	CResourceManager::Load<CModel>("FireShield",		"Wepon\\Spell_Support\\FireShield.obj");
	CResourceManager::Load<CModel>("FireReflector",		"Wepon\\Spell_Support\\FireReflector.obj");
	// 風属性
	CResourceManager::Load<CModel>("WindBall",			"Wepon\\Spell_Attack\\WindBall.obj");
	CResourceManager::Load<CModel>("WindBolt",			"Wepon\\Spell_Attack\\WindBolt.obj");
	CResourceManager::Load<CModel>("WindBreath",		"Wepon\\Spell_Attack\\WindBreath.obj");
	CResourceManager::Load<CModel>("WindTeleport",		"Wepon\\Spell_Support\\WindTeleport.obj");
	CResourceManager::Load<CModel>("WindShield",		"Wepon\\Spell_Support\\WindShield.obj");
	CResourceManager::Load<CModel>("WindReflector",		"Wepon\\Spell_Support\\WindReflector.obj");
	// 地属性
	CResourceManager::Load<CModel>("EarthBall",			"Wepon\\Spell_Attack\\EarthBall.obj");
	CResourceManager::Load<CModel>("EarthBolt",			"Wepon\\Spell_Attack\\EarthBolt.obj");
	CResourceManager::Load<CModel>("EarthBreath",		"Wepon\\Spell_Attack\\EarthBreath.obj");
	CResourceManager::Load<CModel>("EarthTeleport",		"Wepon\\Spell_Support\\EarthTeleport.obj");
	CResourceManager::Load<CModel>("EarthShield",		"Wepon\\Spell_Support\\EarthShield.obj");
	CResourceManager::Load<CModel>("EarthReflector",	"Wepon\\Spell_Support\\EarthReflector.obj");
	// 雷属性
	CResourceManager::Load<CModel>("ThunderBall",		"Wepon\\Spell_Attack\\ThunderBall.obj");
	CResourceManager::Load<CModel>("ThunderBolt",		"Wepon\\Spell_Attack\\ThunderBolt.obj");
	CResourceManager::Load<CModel>("ThunderBreath",		"Wepon\\Spell_Attack\\ThunderBreath.obj");
	CResourceManager::Load<CModel>("ThunderTeleport",	"Wepon\\Spell_Support\\ThunderTeleport.obj");
	CResourceManager::Load<CModel>("ThunderShield",		"Wepon\\Spell_Support\\ThunderShield.obj");
	CResourceManager::Load<CModel>("ThunderReflector",	"Wepon\\Spell_Support\\ThunderReflector.obj");
	// 水属性
	CResourceManager::Load<CModel>("WaterBall",			"Wepon\\Spell_Attack\\WaterBall.obj");
	CResourceManager::Load<CModel>("WaterBolt",			"Wepon\\Spell_Attack\\WaterBolt.obj");
	CResourceManager::Load<CModel>("WaterBreath",		"Wepon\\Spell_Attack\\WaterBreath.obj");
	CResourceManager::Load<CModel>("WaterTeleport",		"Wepon\\Spell_Support\\WaterTeleport.obj");
	CResourceManager::Load<CModel>("WaterShield",		"Wepon\\Spell_Support\\WaterShield.obj");
	CResourceManager::Load<CModel>("WaterReflector",	"Wepon\\Spell_Support\\WaterReflector.obj");
	// 無属性
	CResourceManager::Load<CModel>("NeutralBall",		"Wepon\\Spell_Attack\\NeutralBall.obj");
	CResourceManager::Load<CModel>("NeutralBolt",		"Wepon\\Spell_Attack\\NeutralBolt.obj");
	CResourceManager::Load<CModel>("NeutralBreath",		"Wepon\\Spell_Attack\\NeutralBreath.obj");
	CResourceManager::Load<CModel>("NeutralTeleport",	"Wepon\\Spell_Support\\NeutralTeleport.obj");
	CResourceManager::Load<CModel>("NeutralShield",		"Wepon\\Spell_Support\\NeutralShield.obj");
	CResourceManager::Load<CModel>("NeutralReflector",	"Wepon\\Spell_Support\\NeutralReflector.obj");
	// 当たり判定用のコリジョンモデル
	/*
	効果音
	*/

	// ゲームBGMを読み込み
	//CBGMManager::Instance()->Play(EBGMType::eGame);

	// プレイヤー生成
	CPlayer* player = new CPlayer();
	player->Position(0.0f, 50.0f, 0.0f);

	// 敵の管理クラスを生成
	mpEnemyMgr = new CEnemyManager();

	// CGameCamera2
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 20.0f, 0.0f),
		atPos
	);

	// フィールドクラス生成
	//CTestField* field = new CTestField();

	//// 衝突判定するコライダ―を追加
	//mainCamera->AddCollider(field->GetGroundCol());
	//mainCamera->AddCollider(field->GetWallCol());

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

//シーンの更新処理
void CTestScene::Update()
{
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

	// 敵の管理クラスの更新
	mpEnemyMgr->Update();
}
