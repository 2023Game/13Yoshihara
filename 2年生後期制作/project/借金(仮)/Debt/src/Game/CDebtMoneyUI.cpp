#include "CDebtMoneyUI.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CMoneyManager.h"
#include "CJobStatusManager.h"
#include "CInput.h"
#include "CBGMManager.h"
#include "CTaskManager.h"
#include "CSceneManager.h"

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

// 次へのオフセット座標
#define NEXT_OFFSET_POS CVector2(0.0f, WINDOW_HEIGHT * 0.9f)
// 次へのフォントサイズ
#define NEXT_FONT_SIZE 32
// 次へのテキスト
#define NEXT_TEXT1 "クリックで返済"
#define NEXT_TEXT2 "クリックでスキップ"
#define NEXT_TEXT3 "クリックで閉じる"

// 所持金の減少間隔
#define DECREASE_MONEY_INTERVAL 0.01f
// 減少する値
#define DECREASE_AMOUNT1000 1000	// 1000の位
#define DECREASE_AMOUNT100   100	// 100の位
#define DECREASE_AMOUNT10     10	// 10の位
#define DECREASE_AMOUNT1	   1	// 1の位

// アルファの減少間隔
#define DECREASE_ALPHA_INTERVAL 0.01f
// 減少するアルファ値
#define DECREASE_ALPHA 0.1f
// 減少するサイズ
#define DECREASE_SIZE 16

// コンストラクタ
CDebtMoneyUI::CDebtMoneyUI()
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 3, ETaskPauseType::eMenu)
	, mElapsedTime(0.0f)
	, mResultAmount(0)
	, mFontSize(DAY_FONT_SIZE)
	, mpDayText(nullptr)
	, mpDebtMoneyText(nullptr)
	, mpMoneyText(nullptr)
	, mpBackGround(nullptr)
{
	// お金の管理クラスを取得する
	CMoneyManager* moneyMgr = CMoneyManager::Instance();
	// 既に返済している
	if (moneyMgr->GetDid())
	{
		// 無効にする
		SetEnable(false);
		SetShow(false);
		// これ以下を処理しない
		return;
	}
	// カーソルを出す
	CInput::ShowCursor(true);
	// BGMを設定
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	// ポーズ
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
	// 所持金の減少状態へ
	ChangeState(EState::eDecrease);
	// 所持金
	mMoneyAmount = moneyMgr->GetMoney();
	// 返済額
	mDebtMoneyAmount = moneyMgr->GetDebtMoney();
	// 日数
	int day = moneyMgr->GetDay();
	// 結果
	mResultAmount = mMoneyAmount - mDebtMoneyAmount;

	// 日数のテキストを生成
	mpDayText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// テキストを設定
	mpDayText->ChangeToStr("%d日目\n", day);
	// 揃いを設定
	mpDayText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// オフセット座標を設定
	mpDayText->Position(DAY_OFFSET_POS);
	// フォントサイズを設定
	mpDayText->SetFontSize(mFontSize);

	// 借金額のテキストを生成
	mpDebtMoneyText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// テキストを設定
	mpDebtMoneyText->ChangeToStr("今日の返済額：%d円\n", mDebtMoneyAmount);
	// 揃いを設定
	mpDebtMoneyText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// オフセット座標を設定
	mpDebtMoneyText->Position(DEBT_MONEY_OFFSET_POS);

	// 所持金のテキストを生成
	mpMoneyText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// テキストを設定
	mpMoneyText->ChangeToStr("所持金：%d円\n", mMoneyAmount);
	// 揃いを設定
	mpMoneyText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// オフセット座標を設定
	mpMoneyText->Position(MONEY_OFFSET_POS);

	// 所持金のテキストを生成
	mpNextText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// テキストを設定
	mpNextText->SetStr(NEXT_TEXT1);
	// 揃いを設定
	mpNextText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// オフセット座標を設定
	mpNextText->Position(NEXT_OFFSET_POS);
	// フォントサイズを設定
	mpNextText->SetFontSize(NEXT_FONT_SIZE);
	// フォントカラーを設定
	mpNextText->SetFontColor(CColor(1.0f, 1.0f, 0.5f));
	// 影を設定
	mpNextText->SetShadow(true, CColor(0.25f, 0.125f, 0.0f));
	// アウトラインカラーを設定
	mpNextText->SetOutline(true, CColor(0.0f, 0.0f, 0.0f));
	// 点滅する
	mpNextText->SetBlink(true);

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
	SAFE_DELETE(mpDayText);
	SAFE_DELETE(mpDebtMoneyText);
	SAFE_DELETE(mpMoneyText);
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
	case EState::eGameOver:
		UpdateGameOver();
		break;
	}

	mpDayText->Update();
	mpDebtMoneyText->Update();
	mpMoneyText->Update();
	mpNextText->Update();
}

// 描画
void CDebtMoneyUI::Render()
{
	mpBackGround->Render();
	mpDayText->Render();
	mpDebtMoneyText->Render();
	mpMoneyText->Render();
	// 減少状態なら描画
	if (mState == EState::eDecrease)
	{
		mpNextText->Render();
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
		// 左クリックで次へ
	case 0:
		// 左クリックをしたら
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 次へのテキストを設定
			mpNextText->SetStr(NEXT_TEXT2);
			// 次のステップへ
			mStateStep++;
		}
		break;
		// 所持金を減らす
		// 左クリックで次の状態へ
	case 1:
		mElapsedTime += Times::DeltaTime();
		// 減少間隔の時間が過ぎたら
		if (mElapsedTime >= DECREASE_MONEY_INTERVAL)
		{
			// 所持金を減らす
			DecreaseMoney();
			mElapsedTime -= DECREASE_MONEY_INTERVAL;
			// 返済額が0以下なら
			// もしくは、左クリックでスキップ
			if (mDebtMoneyAmount <= 0 ||
				CInput::PushKey(VK_LBUTTON))
			{
				// 返済額を0にする
				mDebtMoneyAmount = 0;
				// 所持金を結果にする
				mMoneyAmount = mResultAmount;
				// 所持金が0以上なら
				if (mMoneyAmount >= 0)
				{
					// お金の管理クラスを取得
					auto* moneyMgr = CMoneyManager::Instance();
					// 前の日の所持金を設定
					moneyMgr->SetPreMoney(moneyMgr->GetMoney());

					// 仕事のステータス管理クラスを取得
					auto* jobStatusMgr = CJobStatusManager::Instance();
					// 前の日のステータスを設定
					jobStatusMgr->SetPreJobStatus();

					// 返済した
					moneyMgr->SetDid(true);
					// 所持金に結果を設定
					moneyMgr->SetMoney(mMoneyAmount);
				}
				// 次へのテキストを設定
				mpNextText->SetStr(NEXT_TEXT3);
				// 次へ
				mStateStep++;
			}

			// 所持金のテキストを設定
			mpMoneyText->ChangeToStr("所持金：%d円\n", mMoneyAmount);
			// 返済額のテキストを設定
			mpDebtMoneyText->ChangeToStr("返済額：%d円\n", mDebtMoneyAmount);
		}
		break;
	case 2:
		// 左クリックをしたら
		if (CInput::PushKey(VK_LBUTTON))
		{
			// ポーズ解除
			CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
			// 結果が0以上なら
			if (mResultAmount >= 0)
			{
				// お金の管理クラスを取得
				auto* moneyMgr = CMoneyManager::Instance();
				// クリアの日数なら
				if (moneyMgr->IsClear())
				{
					// シーン管理クラスを取得
					auto* sceneMgr = CSceneManager::Instance();
					// ゲームクリアシーンへ
					sceneMgr->LoadScene(EScene::eGameClear);
				}
				else
				{
					// フェードアウト状態へ
					ChangeState(EState::eFadeOut);
					// BGMを設定
					CBGMManager::Instance()->Play(EBGMType::eHome, false);
				}
			}
			// 所持金が0より小さいなら
			else
			{
				// ゲームオーバーへ
				ChangeState(EState::eGameOver);
			}
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
			mpDayText->SetFontSize(mFontSize);
			mpDebtMoneyText->SetFontSize(mFontSize / 2);
			mpMoneyText->SetFontSize(mFontSize / 2);
			// アルファが0以下なら
			if (mpBackGround->GetAlpha() <= 0.0f)
			{
				// カーソルを無効
				CInput::ShowCursor(false);
				// 非表示
				SetEnable(false);
				SetShow(false);
				// 待機状態へ
				ChangeState(EState::eIdle);
			}
		}
		break;
	}
}

// ゲームオーバー
void CDebtMoneyUI::UpdateGameOver()
{
	// ゲームオーバーシーンへ
	CSceneManager::Instance()->LoadScene(EScene::eGameOver);
}

// 状態切り替え
void CDebtMoneyUI::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// 所持金を減らす
void CDebtMoneyUI::DecreaseMoney()
{			
	// 10000以上あるなら
	if (mDebtMoneyAmount >= DECREASE_AMOUNT1000 * 10)
	{
		// 1000減らす
		mMoneyAmount -= DECREASE_AMOUNT1000;
		mDebtMoneyAmount -= DECREASE_AMOUNT1000;
	}
	// 1000以上あるなら
	if (mDebtMoneyAmount >= DECREASE_AMOUNT100 * 10)
	{
		// 100減らす
		mMoneyAmount -= DECREASE_AMOUNT100;
		mDebtMoneyAmount -= DECREASE_AMOUNT100;
	}
	// 100以上あるなら
	if (mDebtMoneyAmount >= DECREASE_AMOUNT10 * 10)
	{
		// 10減らす
		mMoneyAmount -= DECREASE_AMOUNT10;
		mDebtMoneyAmount -= DECREASE_AMOUNT10;
	}
	// 1減らす
	mMoneyAmount -= DECREASE_AMOUNT1;
	mDebtMoneyAmount -= DECREASE_AMOUNT1;
}
