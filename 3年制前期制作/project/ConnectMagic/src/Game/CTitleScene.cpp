#include "CTitleScene.h"
#include "CSceneManager.h"
#include "CCamera.h"
#include "CSound.h"
#include "CBGMManager.h"
#include "CTitleUI.h"
#include "CTaskManager.h"

#include "Test/CTestScene.h"

//コンストラクタ
CTitleScene::CTitleScene()
	: CSceneBase(EScene::eTitle)
{
}

//デストラクタ
CTitleScene::~CTitleScene()
{
}

//シーン読み込み
void CTitleScene::Load()
{
	// タイトル画面はカーソル表示
	CInput::ShowCursor(true);
	// 背景色設定
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//ここでタイトル画面に必要な
	//リソースの読み込みやクラスの生成を行う

	// タイトルBGMを再生
	CBGMManager::Instance()->Play(EBGMType::eTitle);	

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpTitleUI = new CTitleUI();
	AddTask(mpTitleUI);
}

//シーンの更新処理
void CTitleScene::Update()
{
	// タイトル画面が
	if (mpTitleUI->IsEnd())
	{
		// ゲーム開始ならば、ゲームのシーンを読み込む
		if (mpTitleUI->IsStartGame())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// ゲーム終了ならば、アプリを閉じる
		else if (mpTitleUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
