#include "CHomeScene.h"
#include "CSceneManager.h"
#include "CHomeField.h"
#include "CHomePlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"

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
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");

	// 拠点
	CResourceManager::Load<CModel>("HomeBase", "Field\\HomeBase\\HomeBase.obj");
	CResourceManager::Load<CModel>("Bed", "Field\\Object\\Bed.obj");
	CResourceManager::Load<CModel>("Chair", "Field\\Object\\Chair.obj");
	CResourceManager::Load<CModel>("Computer", "Field\\Object\\Computer.obj");
	CResourceManager::Load<CModel>("Desk", "Field\\Object\\Desk.obj");
	CResourceManager::Load<CModel>("Door", "Field\\Object\\Door.obj");
	CResourceManager::Load<CModel>("Rack", "Field\\Object\\Rack.obj");
	// 当たり判定用のコリジョンモデル
	CResourceManager::Load<CModel>("HomeBase_Ground&Ceiling_Collision", "Field\\HomeBase\\CollisionModel\\HomeBase_Ground&Ceiling_Collision.obj");
	CResourceManager::Load<CModel>("HomeBase_Wall_Collision", "Field\\HomeBase\\CollisionModel\\HomeBase_Wall_Collision.obj");
	CResourceManager::Load<CModel>("HomeBase_Object_Collision", "Field\\HomeBase\\CollisionModel\\HomeBase_Object_Collision.obj");


	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eHome);

	new CHomeField();

	CHomePlayer* player = new CHomePlayer();
	player->Scale(1.0f, 1.0f, 1.0f);

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
		atPos + CVector(0.0f, 0.0f, 15.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(player);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

//シーンの更新処理
void CHomeScene::Update()
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
}
