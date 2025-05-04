#include "CScoreUIBase.h"
#include "CTextUI2D.h"

// スコアUIのオフセット座標
#define SCORE_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.35f,WINDOW_HEIGHT - 100.0f,0.0f)

// スコア表示UIのインスタンス
CScoreUIBase* CScoreUIBase::spInstance = nullptr;

// スコア表示UIのインスタンスを取得
CScoreUIBase* CScoreUIBase::Instance()
{
	return spInstance;
}

// コンストラクタ
CScoreUIBase::CScoreUIBase()
{
	// インスタンスを設定
	spInstance = this;
	// スコアのUIを生成
	mpScoreUI = new CTextUI2D();
	// 文字の揃いの基準を設定
	mpScoreUI->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// 座標を設定
	mpScoreUI->Position(SCORE_UI_OFFSET_POS);
}

// デストラクタ
CScoreUIBase::~CScoreUIBase()
{
	SAFE_DELETE(spInstance);
}

// 更新
void CScoreUIBase::Update()
{
	// 描画する文字列を設定
	mpScoreUI->ChangeToStr("Score：%d\n", GetScore());
}
