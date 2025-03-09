#include "CEndUIBase.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"
#include "CMoneyManager.h"
#include "CJobStatusManager.h"

// メニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f
// メニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

#define TEXT_OFFSET_POS CVector2(0.0f, 20.0f)

// コンストラクタ
CEndUIBase::CEndUIBase(std::string bgPath)
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// テキストを生成
	mpText = new CTextUI2D(ETaskPauseType::eGame, false);
	mpText->SetFontSize(128);
	mpText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	mpText->Position(TEXT_OFFSET_POS);

	// 背景イメージを生成
	mpBg = new CImage
	(
		bgPath.c_str(),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpBg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// デストラクタ
CEndUIBase::~CEndUIBase()
{
	SAFE_DELETE(mpText);
	SAFE_DELETE(mpBg);

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
bool CEndUIBase::IsEnd() const
{
	return mIsEnd;
}

// 更新
void CEndUIBase::Update()
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

	mpText->Update();
	mpBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// 描画
void CEndUIBase::Render()
{
	// 背景描画
	mpBg->Render();
	// テキスト描画
	mpText->Render();

	// メニューボタンを表示
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// メニューを開く
void CEndUIBase::UpdateOpen()
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
void CEndUIBase::UpdateSelect()
{
}

// フェードアウト
void CEndUIBase::UpdateFadeOut()
{
}

// 状態切り替え
void CEndUIBase::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}