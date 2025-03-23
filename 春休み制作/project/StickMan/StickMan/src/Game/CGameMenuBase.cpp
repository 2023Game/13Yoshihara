#include "CGameMenuBase.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CSoundManager.h"
#include "CTextUI2D.h"
#include "Easing.h"
#include "CSceneManager.h"

#define MENU_ALPHA 0.75f

// 効果音の音量
#define SE_VOLUME 0.5f

// サイズの倍率
#define SIZE_RATIO 1.5f
// ボタンのサイズ
#define BUTTON_SIZE CVector2(340.0f, 96.0f)*1.5f
// ボタンのオフセット座標
#define BUTTON_OFFSET_POSX 200.0f

// メニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f
// メニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.1f

// テキストのオフセット座標
#define TEXT_OFFSET_POSY 50.0f

// コンストラクタ
CGameMenuBase::CGameMenuBase(std::vector<std::string> menuItemPathList)
	: CTask(ETaskPriority::eUI, 1, ETaskPauseType::eMenu)
	, mIsOpened(false)
	, mpPreMenu(nullptr)
	, mIsNextMenu(false)
	, mElapsedTime(0.0f)
{
	// 待機状態
	ChangeState(EState::eIdle);
	// 効果音
	/*mpSelectSE = CResourceManager::Get<CSound>("SelectSE");
	mpPushSE = CResourceManager::Get<CSound>("PushSE");
	mpBuzzerSE = CResourceManager::Get<CSound>("BuzzerSE");*/

	int menuItemMax = menuItemPathList.size();

	mpBackground = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetSize(mpBackground->GetSize() * SIZE_RATIO);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);


	int menuItemCount = menuItemMax;
	float spaceY = (float)WINDOW_HEIGHT / (menuItemCount + 1);
	for (int i = 0; i < menuItemCount; i++)
	{
		// ボタンを生成
		CExpandButton* btn = new CExpandButton
		(
			CVector2(0.0f, 0.0f),
			BUTTON_SIZE,
			ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
			false, false
		);
		// 座標を設定
		float posX = (1280.0f - 1024.0f + btn->GetSize().X()) * 0.5f + BUTTON_OFFSET_POSX;
		btn->SetPos(posX, spaceY * (i + 1));
		// ボタンの画像を読み込み
		btn->LoadButtonImage(menuItemPathList[i], menuItemPathList[i]);
		// ボタンは最初は無効化して、スケール値を0にしておく
		btn->SetEnable(false);
		btn->SetScale(0.0f);
		// 最後の要素なら
		if (i == menuItemCount - 1)
		{
			btn->SetOnClickFunc(std::bind(&CGameMenuBase::OnClickClose, this));
		}
		// ボタンリストに追加
		mButtons.push_back(btn);
		mMenuOnOff.push_back(true);

		// テキストを生成
		CTextUI2D* text = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
		text->Position(WINDOW_WIDTH * 0.5f, spaceY * (i + 1) - TEXT_OFFSET_POSY, 0.0f);
		mMenuTexts.push_back(text);
	}

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CGameMenuBase::~CGameMenuBase()
{
}

// メニューを開く
void CGameMenuBase::Open()
{
	SetEnable(true);
	SetShow(true);
	mIsOpened = true;
	mElapsedTime = 0.0f;
	if (mpPreMenu == nullptr)
	{
		CBGMManager::Instance()->Play(EBGMType::eMenu, false, 0.25f);
		CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
		// メニューはカーソル表示
		CInput::ShowCursor(true);
	}
	// メニューを開く状態
	ChangeState(EState::eOpen);
	for (CButton* btn : mButtons)
	{	
		// ボタンは無効化して、スケール値を0にしておく
		btn->SetEnable(false);
		btn->SetScale(0.0f);
	}
}

// メニューを閉じる
void CGameMenuBase::Close()
{
	SetEnable(false);
	SetShow(false);
	mIsOpened = false;

	if (mpPreMenu == nullptr)
	{
		CBGMManager::Instance()->Play(EBGMType::eGame, false, 0.25f);
		CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
		// カーソル非表示
		CInput::ShowCursor(false);
	}	// 待機状態
	ChangeState(EState::eIdle);
}

// メニューが開いているかどうか
bool CGameMenuBase::IsOpened() const
{
	return mIsOpened;
}

// 更新
void CGameMenuBase::Update()
{
	switch (mState)
	{
		// 待機
	case EState::eIdle:
		UpdateIdle();
		break;
		// メニューを開く
	case EState::eOpen:
		UpdateOpen();
		break;
		// メニュー選択
	case EState::eSelect:
		UpdateSelect();
		break;
	}

	mpBackground->Update();
	// アイテムとテキストのサイズは一緒
	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		mButtons[i]->Update();
		mMenuTexts[i]->Update();
	}
}

// 描画
void CGameMenuBase::Render()
{
	mpBackground->Render();
	// アイテムとテキストのサイズは一緒
	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CExpandButton* btn = mButtons[i];
		CTextUI2D* text = mMenuTexts[i];
		// オフの場合暗くする
		if (!mMenuOnOff[i])
		{
			btn->SetColor(0.1f, 0.1f, 0.1f);
		}
		// オンの場合明るくする
		else
		{
			btn->SetColor(1.0f, 1.0f, 1.0f);
		}
		btn->Render();
		text->Render();
	}
}

// メニューの要素の有効無効を設定
void CGameMenuBase::SetMenuOnOff(int num, bool isOnOff)
{
	mMenuOnOff[num] = isOnOff;
}

// 一つ前のメニューを設定
void CGameMenuBase::SetPreMenu(CGameMenuBase* preMenu)
{
	mpPreMenu = preMenu;
}

// 有効無効の切り替え
void CGameMenuBase::SetOnOff(bool isOnOff)
{
	SetEnable(isOnOff);
	SetShow(isOnOff);
}

// 待機
void CGameMenuBase::UpdateIdle()
{
}

// メニューを開く
void CGameMenuBase::UpdateOpen()
{
	switch (mStateStep)
	{
		// ステップ0：メニューの入場アニメーション
	case 0:
		if (mElapsedTime < OPEN_ANIM_TIME)
		{
			// スケール値を一旦1.0より大きくして、1.0へ戻るイージングアニメーション
			float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(scale);
			}
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(1.0f);
			}
			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
		// ステップ1：メニュー入場後の待ち
	case 1:
		if (mElapsedTime < OPENED_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// 一定時間待ったら、ボタンをオンにしてタッチできるようにする
			// （誤タッチを防ぐための待ち時間）
			for (CExpandButton* btn : mButtons)
			{
				btn->SetEnable(true);
			}
			ChangeState(EState::eSelect);
		}
		break;
	}
}

// メニュー選択
void CGameMenuBase::UpdateSelect()
{
}

// 状態切り替え
void CGameMenuBase::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [CLOSE]クリック時のコールバック関数
void CGameMenuBase::OnClickClose()
{
	// プッシュ音
	//mpPushSE->Play(SE_VOLUME, true);
	
	// 前のメニューがあるならそれを有効
	if (mpPreMenu != nullptr)
	{
		mpPreMenu->SetOnOff(true);
	}
	Close();
}
