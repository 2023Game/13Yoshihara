#include "CResultUIBase.h"
#include "CFont.h"
#include "CTextUI2D.h"
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
#define BUTTON_OFFSET_POS CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.25f)
// ボタンのサイズ
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// スコアというテキストのオフセット座標
#define SCORE_TEXT_OFFSET_POS			CVector2(0.0f,25.0f)
// 得点のテキストのオフセット座標
#define SCORE_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.0f,   WINDOW_HEIGHT * 0.2f)
// ゴミ袋の数のテキストのオフセット座標
#define NUM_OFFSET_POS					CVector2(WINDOW_WIDTH * 0.3f,   WINDOW_HEIGHT * 0.4f)
// ゴミ袋の値段のテキストのオフセット座標
#define PRICE_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.475f, WINDOW_HEIGHT * 0.4f)
// ゴミ袋の値段のテキストのオフセット座標
#define TOTAL_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.7f,   WINDOW_HEIGHT * 0.4f)
// 単位のテキストのオフセット座標
#define UNIT_OFFSET_POS					CVector2(WINDOW_WIDTH * 0.35f,  WINDOW_HEIGHT * 0.4f)
// 記号のテキストのオフセット座標
#define SYMBOL_OFFSET_POS				CVector2(WINDOW_WIDTH * 0.42f,  WINDOW_HEIGHT * 0.4f)
// 一番下の計算の説明テキストのオフセット座標
#define DESCRIPTION_OFFSET_POS			CVector2(WINDOW_WIDTH * 0.15f, WINDOW_HEIGHT * 0.76f)
// ボーナスの説明テキストのオフセット座標
#define BONUS_DESCRIPTION_OFFSET_POS	CVector2(WINDOW_WIDTH * 0.475f, WINDOW_HEIGHT * 0.83f)


// コンストラクタ
CResultUIBase::CResultUIBase(std::string backGroundPath)
	: CEndUIBase(backGroundPath)
{
	// テキストを設定
	mpText->SetStr("Score");
	mpText->SetFontSize(116);
	mpText->Position(SCORE_TEXT_OFFSET_POS);

	// 得点のテキストを生成
	mpScorePointText = new CTextUI2D(ETaskPauseType::eGame, false);
	// 座標を設定
	mpScorePointText->Position(SCORE_OFFSET_POS);
	// フォントサイズを設定
	mpScorePointText->SetFontSize(128);
	// 文字の揃いの基準を設定
	mpScorePointText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);

	// 数値のテキスト1を生成
	mpNumText1 = new CTextUI2D(ETaskPauseType::eGame, false);
	// 座標を設定
	mpNumText1->Position(NUM_OFFSET_POS);

	// 数値のテキスト2を生成
	mpNumText2 = new CTextUI2D(ETaskPauseType::eGame, false);
	// 座標を設定
	mpNumText2->Position(PRICE_OFFSET_POS);

	// 計算結果のテキストを生成
	mpTotalText = new CTextUI2D(ETaskPauseType::eGame, false);
	// 座標を設定
	mpTotalText->Position(TOTAL_OFFSET_POS);

	// 単位のテキストを生成
	mpUnitText = new CTextUI2D(ETaskPauseType::eGame, false);
	// 単位のテキストを設定
	mpUnitText->SetStr("コ　　　　　　円\n　\n　\nコ　　　　　　円\n");
	// 座標を設定
	mpUnitText->Position(UNIT_OFFSET_POS);

	// 記号のテキストを生成
	mpSymbolText = new CTextUI2D(ETaskPauseType::eGame, false);
	// 記号のテキストを設定
	mpSymbolText->SetStr("×　　　　　　＝\n　\n　\n×　　　　　　＝\n　\n　\n×　　　　　　＝\n");
	// 座標を設定
	mpSymbolText->Position(SYMBOL_OFFSET_POS);

	// 一番下の計算の説明テキスト
	mpDescriptionText = new CTextUI2D(ETaskPauseType::eGame, false);
	//  一番下の計算の説明テキストを設定
	mpDescriptionText->SetStr("ボーナス！！");
	// 座標を設定
	mpDescriptionText->Position(DESCRIPTION_OFFSET_POS);
	// 文字の色を設定
	mpDescriptionText->SetFontColor(CColor(0.9f, 0.0f, 0.0f));

	// ボーナスの説明テキスト
	mpBonusDescriptionText = new CTextUI2D(ETaskPauseType::eGame, false, nullptr);
	// フォントサイズを設定
	mpBonusDescriptionText->SetFontSize(32);
	// 座標を設定
	mpBonusDescriptionText->Position(BONUS_DESCRIPTION_OFFSET_POS);

	// [拠点へ]ボタンを生成
	CExpandButton* btn = new CExpandButton
	(
		BUTTON_OFFSET_POS,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn->LoadButtonImage("UI/result_return0.png", "UI/result_return1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn->SetOnClickFunc(std::bind(&CResultUIBase::OnClickReturn, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn->SetEnable(false);
	btn->SetScale(0.0f);
	// ボタンリストに追加
	mButtons.push_back(btn);
}

// デストラクタ
CResultUIBase::~CResultUIBase()
{
	SAFE_DELETE(mpScorePointText);
	SAFE_DELETE(mpNumText1);
	SAFE_DELETE(mpNumText2);
	SAFE_DELETE(mpUnitText);
	SAFE_DELETE(mpSymbolText);
	SAFE_DELETE(mpDescriptionText);
	SAFE_DELETE(mpBonusDescriptionText);
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
	CEndUIBase::Update();

	// テキスト
	mpScorePointText->Update();
	mpNumText1->Update();
	mpNumText2->Update();
	mpTotalText->Update();
	mpUnitText->Update();
	mpSymbolText->Update();
	mpDescriptionText->Update();
	mpBonusDescriptionText->Update();
}

// 描画
void CResultUIBase::Render()
{
	CEndUIBase::Render();

	// テキスト描画
	mpScorePointText->Render();
	mpNumText1->Render();
	mpNumText2->Render();
	mpTotalText->Render();
	mpUnitText->Render();
	mpSymbolText->Render();
	mpDescriptionText->Render();
	mpBonusDescriptionText->Render();
}

// スコアを取得
int CResultUIBase::GetScore() const
{
	return mScore;
}

// スコアを設定
void CResultUIBase::SetScore(int score)
{
	mScore = score;
}