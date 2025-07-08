#include "CSelectScene.h"
#include "CSceneManager.h"
#include "CInput.h"
#include "CBGMManager.h"
#include "CSelectMenu.h"

CSelectScene::CSelectScene()
	: CSceneBase(EScene::eSelect)
{
}

// デストラクタ
CSelectScene::~CSelectScene()
{
}

// シーン読み込み
void CSelectScene::Load()
{
	// タイトル画面はカーソル表示
	CInput::ShowCursor(true);
	// 背景色設定
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// タイトルBGMを再生
	CBGMManager::Instance()->Play(EBGMType::eTitle);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpSelectUI = new CSelectMenu();
	AddTask(mpSelectUI);
}

// シーンの更新処理
void CSelectScene::Update()
{
	// 選択画面が
	if (mpSelectUI->IsEnd())
	{
		// 対戦開始ならば、対戦シーンを読み込む
		if (mpSelectUI->IsBattleStart())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGameTest);
		}
		// 戻るならば、タイトルシーンを読み込む
		else if (mpSelectUI->IsReturn())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
	}
}
