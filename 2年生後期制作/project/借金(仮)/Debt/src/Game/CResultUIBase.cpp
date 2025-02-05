#include "CResultUIBase.h"
#include "CFont.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// タイトルメニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f
// タイトルメニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

// ボタンのオフセット座標
#define BUTTON_POS CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.8f)
// ボタンのサイズ
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// テキストのオフセット座標
#define TEXT_POS CVector(0.0f,25.0f,0.0f)

// コンストラクタ
CResultUIBase::CResultUIBase(std::string backGroundPath)
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
	, mSelectIndex(-1)
{
	// スコアのテキストを生成
	mpScoreText = new CTextUI2D(ETaskPauseType::eGame, false);
	mpScoreText->SetStr("Score");
	mpScoreText->SetFontSize(116);
	mpScoreText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	mpScoreText->Position(TEXT_POS);

	// リザルト画面の背景イメージを生成
	mpResultBg = new CImage
	(
		backGroundPath.c_str(),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// [拠点へ]ボタンを生成
	mpReturnButton = new CExpandButton
	(
		BUTTON_POS,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	mpReturnButton->LoadButtonImage("UI/result_return0.png", "UI/result_return1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	mpReturnButton->SetOnClickFunc(std::bind(&CResultUIBase::OnClickReturn, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	mpReturnButton->SetEnable(false);
	mpReturnButton->SetScale(0.0f);
}

// デストラクタ
CResultUIBase::~CResultUIBase()
{
	SAFE_DELETE(mpScoreText);
	SAFE_DELETE(mpResultBg);
}

// 結果画面終了か
bool CResultUIBase::IsEnd() const
{
	return mIsEnd;
}

// 拠点へ戻るか
bool CResultUIBase::IsReturnHome() const
{
	// 選択項目が1つ目ならば、拠点へ戻る
	return mSelectIndex == 0;
}

// [RETURN]クリック時のコールバック関数
void CResultUIBase::OnClickReturn()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}


// 更新
void CResultUIBase::Update()
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
	mpScoreText->Update();
	mpResultBg->Update();
	mpReturnButton->Update();
}

// 描画
void CResultUIBase::Render()
{
	// 描画順
	// 背景→スコア→拠点へ戻るボタン
	
	// 背景描画
	mpResultBg->Render();
	// スコアの文字描画
	mpScoreText->Render();
	// ボタン描画
	mpReturnButton->Render();
}

// メニューを開く
void CResultUIBase::UpdateOpen()
{
	switch (mStateStep)
	{
		// ステップ0：メニューの入場アニメーション
	case 0:
		if (mElapsedTime < OPEN_ANIM_TIME)
		{
			// スケール値を一旦1.0より大きくして、1.0へ戻るイージングアニメーション
			float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
			mpReturnButton->SetScale(scale);
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			mpReturnButton->SetScale(1.0f);
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
			mpReturnButton->SetEnable(true);
			ChangeState(EState::eSelect);
		}
		break;
	}
}

// メニュー選択
void CResultUIBase::UpdateSelect()
{
}

// フェードアウト
void CResultUIBase::UpdateFadeOut()
{
}

// 状態切り替え
void CResultUIBase::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}