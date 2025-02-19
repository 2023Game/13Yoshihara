#include "CGameOverUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// メニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f
// メニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

// ボタンのオフセット座標
#define BUTTON_OFFSET_POS1 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.5f)
#define BUTTON_OFFSET_POS2 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.75f)
// ボタンのサイズ
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// コンストラクタ
CGameOverUI::CGameOverUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// フォントデータを生成
	mpFont = new CFont("res\\Font\\toroman.ttf");
	mpFont->SetFontSize(128);
	mpFont->SetAlignment(FTGL::TextAlignment::ALIGN_CENTER);
	mpFont->SetLineLength(WINDOW_WIDTH);

	// ゲームオーバーのテキストを生成
	mpGameOverText = new CText
	(
		mpFont, 128,
		CVector2(0.0f, 20.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(0.1f,0.1f,0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpGameOverText->SetText("ゲームオーバー");
	mpGameOverText->SetEnableOutline(true);
	mpGameOverText->SetOutlineColor(CColor(0.9f, 0.9f, 0.9f));

	// ゲームオーバー画面の背景イメージを生成
	mpGameOverBg = new CImage
	(
		"UI/game_over_bg.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpGameOverBg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// [前日へ]ボタンを生成
	CExpandButton* btn1 = new CExpandButton
	(
		BUTTON_OFFSET_POS1,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn1->LoadButtonImage("UI/previous_day0.png", "UI/previous_day1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn1->SetOnClickFunc(std::bind(&CGameOverUI::OnClickPreDay, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn1->SetEnable(false);
	btn1->SetScale(0.0f);
	// ボタンリストに追加
	mButtons.push_back(btn1);

	// [終了]ボタンを生成
	CExpandButton* btn2 = new CExpandButton
	(
		BUTTON_OFFSET_POS2,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn2->LoadButtonImage("UI/title_quit0.png", "UI/title_quit1.png");
	btn2->SetOnClickFunc(std::bind(&CGameOverUI::OnClickQuit, this));
	btn2->SetEnable(false);
	btn2->SetScale(0.0f);
	mButtons.push_back(btn2);
}

// デストラクタ
CGameOverUI::~CGameOverUI()
{
	SAFE_DELETE(mpFont);
	SAFE_DELETE(mpGameOverText);
	SAFE_DELETE(mpGameOverBg);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// ゲームオーバー画面終了か
bool CGameOverUI::IsEnd() const
{
	return mIsEnd;
}

// 前日へ戻るか
bool CGameOverUI::IsPreDay() const
{
	// 選択項目が1つ目ならば、前日へ
	return mSelectIndex == 0;
}

// ゲームを終了するか
bool CGameOverUI::IsExitGame() const
{
	// 選択項目が2つ目ならば、ゲーム終了
	return mSelectIndex == 1;
}

// 更新
void CGameOverUI::Update()
{
	switch (mState)
	{
		// メニューを開く
	case EState::eOpen:
		UpdateOpen();
		break;
		// メニュー選択
	case EState::eSelect:
		UpdateSelect();
		break;
		// フェードアウト
	case EState::eFadeOut:
		UpdateFadeOut();
		break;
	}

	mpGameOverText->Update();
	mpGameOverBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// 描画
void CGameOverUI::Render()
{
	// 背景描画
	mpGameOverBg->Render();
	// ゲームオーバーテキスト描画
	mpGameOverText->Render();

	// メニューボタンを表示
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// メニューを開く
void CGameOverUI::UpdateOpen()
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
void CGameOverUI::UpdateSelect()
{
}

// フェードアウト
void CGameOverUI::UpdateFadeOut()
{
}

// 状態切り替え
void CGameOverUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [前日へ]クリック時のコールバック関数
void CGameOverUI::OnClickPreDay()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [終了]クリック時のコールバック関数
void CGameOverUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}
