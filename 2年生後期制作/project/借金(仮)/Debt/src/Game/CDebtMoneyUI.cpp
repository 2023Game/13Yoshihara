#include "CDebtMoneyUI.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CMoneyManager.h"
#include "CInput.h"

// 背景画像のパス
#define BG_PATH "UI/white.png"

// 日数のオフセット座標
#define DAY_OFFSET_POS CVector2(0.0f, WINDOW_HEIGHT * 0.1f)
// 日数のフォントサイズ
#define DAY_FONT_SIZE 128

// 借金額のオフセット座標
#define DEBT_MONEY_OFFSET_POS CVector2(0.0f, WINDOW_HEIGHT * 0.4f)

// 所持金のオフセット座標
#define MONEY_OFFSET_POS CVector2(0.0f, WINDOW_HEIGHT * 0.8f)

// 所持金の減少間隔
#define DECREASE_MONEY_INTERVAL 0.01f
// 減少する値
#define DECREASE_AMOUNT 20

// アルファの減少間隔
#define DECREASE_ALPHA_INTERVAL 0.01f
// 減少するアルファ値
#define DECREASE_ALPHA 0.2f
// 減少するサイズ
#define DECREASE_SIZE 16

// コンストラクタ
CDebtMoneyUI::CDebtMoneyUI()
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mElapsedTime(0.0f)
	, mDecreaseAmount(0)
	, mFontSize(DAY_FONT_SIZE)
{
	// 所持金の減少状態へ
	ChangeState(EState::eDecrease);
	// 借金額のテキストを生成
	mpDay = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// テキストを設定
	mpDay->ChangeToStr("%d日目\n", 0);
	// 揃いを設定
	mpDay->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// オフセット座標を設定
	mpDay->Position(DAY_OFFSET_POS);
	// フォントサイズを設定
	mpDay->SetFontSize(mFontSize);

	// 借金額のテキストを生成
	mpDebtMoney = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// テキストを設定
	mpDebtMoney->ChangeToStr("今日の返済額：%d円\n", 1000);
	// 返済額を保存
	mDebtMoneyAmount = 1000;
	// 揃いを設定
	mpDebtMoney->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// オフセット座標を設定
	mpDebtMoney->Position(DEBT_MONEY_OFFSET_POS);

	// 所持金を取得する
	CMoneyManager* moneyMgr = CMoneyManager::Instance();
	int money = moneyMgr->GetMoney();
	// 所持金のテキストを生成
	mpMoney = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// テキストを設定
	mpMoney->ChangeToStr("所持金：%d円\n", money);
	// 所持金を保存
	mMoneyAmount = money;
	// 揃いを設定
	mpMoney->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// オフセット座標を設定
	mpMoney->Position(MONEY_OFFSET_POS);

	mpBackGround = new CImage
	(
		BG_PATH,
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpBackGround->SetColor(CColor::black);
	mpBackGround->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// デストラクタ
CDebtMoneyUI::~CDebtMoneyUI()
{
	SAFE_DELETE(mpDebtMoney);
	SAFE_DELETE(mpMoney);
	SAFE_DELETE(mpBackGround);
}

// 更新
void CDebtMoneyUI::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eDecrease:
		UpdateDecrease();
		break;
	case EState::eFadeOut:
		UpdateFadeOut();
		break;
	}

	mpDay->Update();
	mpDebtMoney->Update();
	mpMoney->Update();
}

// 描画
void CDebtMoneyUI::Render()
{
	mpBackGround->Render();
	// 描画するなら
	if (mpDay->IsShow())
	{
		mpDay->Render();
	}
	if (mpDebtMoney->IsShow())
	{
		mpDebtMoney->Render();
	}
	if (mpMoney->IsShow())
	{
		mpMoney->Render();
	}
}

// 待機
void CDebtMoneyUI::UpdateIdle()
{
}

// 所持金の減少
void CDebtMoneyUI::UpdateDecrease()
{
	switch (mStateStep)
	{
	case 0:
		mElapsedTime += Times::DeltaTime();
		// 減少間隔の時間が過ぎたら
		if (mElapsedTime >= DECREASE_MONEY_INTERVAL)
		{
			// 減少する値分減らす
			mMoneyAmount -= DECREASE_AMOUNT;
			// 減少させたので減少した値を増やす
			mDecreaseAmount += DECREASE_AMOUNT;
			mElapsedTime -= DECREASE_MONEY_INTERVAL;
			// 減少した値が返済額になれば
			if (mDebtMoneyAmount == mDecreaseAmount)
			{
				// 次のステップへ
				mStateStep++;
			}
			// 所持金のテキストを設定
			mpMoney->ChangeToStr("所持金：%d円\n", mMoneyAmount);
		}
		break;

	case 1:
		// 左クリックをしたら
		if (CInput::PushKey(VK_LBUTTON))
		{
			// フェードアウト状態へ
			ChangeState(EState::eFadeOut);
		}
		break;
	}
}

// フェードアウト
void CDebtMoneyUI::UpdateFadeOut()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// フェードアウトする
	case 0:
		// 減少間隔の時間が過ぎたら
		if (mElapsedTime >= DECREASE_ALPHA_INTERVAL)
		{
			mElapsedTime -= DECREASE_ALPHA_INTERVAL;
			// フォントサイズを減少
			mFontSize -= DECREASE_SIZE;
			// アルファを減少
			mpBackGround->SetAlpha(mpBackGround->GetAlpha() - DECREASE_ALPHA);
			mpDay->SetFontSize(mFontSize);
			mpDebtMoney->SetFontSize(mFontSize / 2);
			mpMoney->SetFontSize(mFontSize / 2);
			// アルファが0以下なら
			if (mpBackGround->GetAlpha() <= 0.0f)
			{
				// 非表示
				mpDay->SetShow(false);
				mpDebtMoney->SetShow(false);
				mpMoney->SetShow(false);
				// 待機状態へ
				ChangeState(EState::eIdle);
			}
		}
		break;
	}
}

// 状態切り替え
void CDebtMoneyUI::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}
