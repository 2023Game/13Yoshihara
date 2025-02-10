#include "CResultScene.h"
#include "CSceneManager.h"
#include "CHomeScene.h"
#include "CCamera.h"
#include "CSound.h"
#include "CBGMManager.h"
#include "CResultUIBase.h"
#include "CTrashGameResultUI.h"
#include "CDeliveryGameResultUI.h"
#include "CScoreManager.h"
#include "SceneType.h"
#include "CTaskManager.h"
#if _DEBUG
#include "CInput.h"
#endif

// コンストラクタ
CResultScene::CResultScene()
	: CSceneBase(EScene::eResult)
{
}

// デストラクタ
CResultScene::~CResultScene()
{
}

// シーン読み込み
void CResultScene::Load()
{
	// ポーズを解除
	CTaskManager::Instance()->UnPause(PAUSE_GAME);
	// タイトル画面はカーソル表示
	CInput::ShowCursor(true);
	// 背景色設定
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//ここでリザルト画面に必要な
	//リソースの読み込みやクラスの生成を行う

	// TODO：リザルトBGMを再生
	CBGMManager::Instance()->Play(EBGMType::eTitle);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	CScoreManager* scoreMgr = CScoreManager::Instance();
	// ゲームの種類がゴミ拾いなら
	if (scoreMgr->GetGameType() == (int)EScene::eTrashGame)
	{
		// ゴミ拾いのリザルトUIを生成
		mpResultUI = new CTrashGameResultUI();
	}
	if (scoreMgr->GetGameType() == (int)EScene::eDeliveryGame)
	{
		// 配達のリザルトUIを生成
		mpResultUI = new CDeliveryGameResultUI();
	}
	AddTask(mpResultUI);
}

// シーンの更新処理
void CResultScene::Update()
{
	// タイトル画面が
	if (mpResultUI->IsEnd())
	{
		// 拠点へ帰るならば、拠点シーンを読み込む
		if (mpResultUI->IsReturnHome())
		{
			CSceneManager::Instance()->LoadScene(EScene::eHome);
		}
	}
#if _DEBUG
	if (CInput::PushKey('1'))
	{
		// ゴミ拾い
		CScoreManager::Instance()->SetGameType((int)EScene::eTrashGame);
		// 読み込みなおし
		CSceneManager::Instance()->LoadScene(EScene::eResult);
	}
	else if (CInput::PushKey('2'))
	{
		// 配達
		CScoreManager::Instance()->SetGameType((int)EScene::eDeliveryGame);
		// 読み込みなおし
		CSceneManager::Instance()->LoadScene(EScene::eResult);
	}
#endif
}
