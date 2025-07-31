#include "CGameClearScene.h"
#include "CBGMManager.h"
#include "CGameClearUI.h"
#include "CSceneManager.h"

// コンストラクタ
CGameClearScene::CGameClearScene()
	: CSceneBase(EScene::eClear)
{
}

// デストラクタ
CGameClearScene::~CGameClearScene()
{
}

// シーン読み込み
void CGameClearScene::Load()
{
	// タイトル画面はカーソル表示
	CInput::ShowCursor(true);
	// 背景色設定
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// ゲームクリアBGMを再生
	//CBGMManager::Instance()->Play(EBGMType::eClear);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpGameClearUI = new CGameClearUI();
	AddTask(mpGameClearUI);
}

// シーンの更新処理
void CGameClearScene::Update()
{
	// ゲームクリア画面が
	if (mpGameClearUI->IsEnd())
	{
		// タイトルへ戻るならば、タイトルへ
		if (mpGameClearUI->IsReturnTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
		// ゲーム終了ならば、アプリを閉じる
		else if (mpGameClearUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
