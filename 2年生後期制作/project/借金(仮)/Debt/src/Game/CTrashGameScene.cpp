#include "CTrashGameScene.h"
#include "CSceneManager.h"
#include "CTrashField.h"
#include "CPlayer.h"
#include "CTrashPlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CCarAndTruckManager.h"

//コンストラクタ
CTrashGameScene::CTrashGameScene()
	: CSceneBase(EScene::eTrashGame)
	, mpGameMenu(nullptr)
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

	CResourceManager::Load<CModel>("Field", "Field\\Field.obj");
	CResourceManager::Load<CModel>("FieldCube", "Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>("FieldCylinder", "Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");
	CResourceManager::Load<CTexture>("LightningBolt", "Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>("Slash", "Effect\\slash.obj");
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");
	
	// ゴミ拾い
	CResourceManager::Load<CModelX>("TrashPlayer", "Character\\TrashBox\\TrashBox.x");
	CResourceManager::Load<CModel>("TrashStage", "Field\\TrashStage\\TrashStage.obj");
	CResourceManager::Load<CModel>("TrashBox", "Field\\Object\\TrashBox.obj");
	CResourceManager::Load<CModel>("Car", "Character\\Car\\Car.obj");
	CResourceManager::Load<CModel>("GarbageTruck", "Character\\GarbageTruck\\GarbageTruck.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eHome);

	new CTrashField();

	CPlayer* player = new CPlayer();
	player->Scale(1.0f, 1.0f, 1.0f);

	CTrashPlayer* trashPlayer = new CTrashPlayer();
	trashPlayer->Scale(0.1f, 0.1f, 0.1f);

	// 車とトラックの管理クラス作成
	mpCCarAndTruckManager =
		new CCarAndTruckManager();

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

	mpCCarAndTruckManager->Update();
}
