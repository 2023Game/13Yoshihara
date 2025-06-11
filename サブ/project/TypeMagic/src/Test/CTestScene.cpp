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
#include "CEnemy.h"

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
	CResourceManager::Load<CModel>("FireBall",			"Wepon\\Spell_Attack\\FireBall.obj");
	CResourceManager::Load<CModel>("FireBolt",			"Wepon\\Spell_Attack\\FireBolt.obj");
	CResourceManager::Load<CModel>("FireBreath",		"Wepon\\Spell_Attack\\FireBreath.obj");
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

	CEnemy* enemy = new CEnemy();
	enemy->Position(0.0f, 50.0f, -50.0f);

	// それぞれの相手を設定
	enemy->SetOpponent(player);
	player->SetOpponent(enemy);

	// CGameCamera2
	CVector atPos = player->Position() + CVector(0.0f, 20.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 20.0f, 0.0f),
		atPos
	);

	// フィールドクラス生成
	CTestField* field = new CTestField();

	//// 衝突判定するコライダ―を追加
	mainCamera->AddCollider(field->GetGroundCol());
	mainCamera->AddCollider(field->GetWallCol());

	mainCamera->SetFollowTargetTf(player);
	mainCamera->SetLockOnTarget(enemy);

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
