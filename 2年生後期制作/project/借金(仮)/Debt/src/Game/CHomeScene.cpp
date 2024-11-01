#include "CHomeScene.h"
#include "CSceneManager.h"
#include "CHomeField.h"
#include "CPlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenuBase.h"
#include "CBGMManager.h"
#include "CLineEffect.h"

#define MENU_ITEM1 "UI/menu_item.png"
#define MENU_SELECT "UI/menu_item_select.png"

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

	CResourceManager::Load<CModel>("Field", "Field\\Field.obj");
	CResourceManager::Load<CModel>("FieldCube", "Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>("FieldCylinder", "Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");
	CResourceManager::Load<CTexture>("LightningBolt", "Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>("Slash", "Effect\\slash.obj");
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");

	CResourceManager::Load<CModel>("HomeBase", "Field\\HomeBase\\HomeBase.obj");
	CResourceManager::Load<CModel>("Bed", "Field\\Object\\Bed.obj");
	CResourceManager::Load<CModel>("Chair", "Field\\Object\\Chair.obj");
	CResourceManager::Load<CModel>("Computer", "Field\\Object\\Computer.obj");
	CResourceManager::Load<CModel>("Desk", "Field\\Object\\Desk.obj");
	CResourceManager::Load<CModel>("Door", "Field\\Object\\Door.obj");
	CResourceManager::Load<CModel>("DustBox", "Field\\Object\\DustBox.obj");
	CResourceManager::Load<CModel>("Rack", "Field\\Object\\Rack.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eHome);

	new CHomeField();

	CPlayer* player = new CPlayer();
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

	std::vector < std::string> menuItemPathList;
	menuItemPathList.push_back(MENU_ITEM1);
	menuItemPathList.push_back(MENU_ITEM1);
	menuItemPathList.push_back(MENU_ITEM1);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenuBase(menuItemPathList, MENU_SELECT);
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
