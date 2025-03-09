#include "CGameOverScene.h"
#include "CBGMManager.h"
#include "CGameOverUI.h"
#include "CSceneManager.h"

// コンストラクタ
CGameOverScene::CGameOverScene()
	: CSceneBase(EScene::eGameOver)
{
}

// デストラクタ
CGameOverScene::~CGameOverScene()
{
}

// シーン読み込み
void CGameOverScene::Load()
{
	// タイトル画面はカーソル表示
	CInput::ShowCursor(true);
	// 背景色設定
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// ゲームオーバーBGMを再生
	CBGMManager::Instance()->Play(EBGMType::eGameOver);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpGameOverUI = new CGameOverUI();
	AddTask(mpGameOverUI);
}

// シーンの更新処理
void CGameOverScene::Update()
{
	// ゲームオーバー画面が
	if (mpGameOverUI->IsEnd())
	{
		// 前日へ戻るならば、ホームシーンを読み込む
		if (mpGameOverUI->IsPreDay())
		{
			CSceneManager::Instance()->LoadScene(EScene::eHome);
		}
		// ゲーム終了ならば、アプリを閉じる
		else if (mpGameOverUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
