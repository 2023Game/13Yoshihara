#include "CRewindUI.h"
#include "CImage.h"
#include "CTextUI2D.h"
#include "CGaugeUI2D.h"

// 画像のパス
const char* const IMG_PATH =	"UI\\white.png";
// 画像のアルファ
constexpr float ALPHA =			0.1f;
// 文字座標
const CVector TEXT_POS =		CVector(0.0f, 120.0f, 0.0f);
// 文字サイズ
constexpr int TEXT_FONT_SIZE =	128;

// コンストラクタ
CRewindUI::CRewindUI()
	: CTask(ETaskPriority::eSystem, 0, ETaskPauseType::eSystem, false, true)
{
	// 画像
	mpImg = new CImage(IMG_PATH, ETaskPriority::eSystem, 0, ETaskPauseType::eSystem, false, false);
	mpImg->SetColor(CColor::red);
	mpImg->SetAlpha(ALPHA);
	mpImg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// テキスト
	mpDangerText = new CTextUI2D(ETaskPauseType::eSystem, false);
	mpDangerText->SetStr("DANGER");
	mpDangerText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	mpDangerText->Position(TEXT_POS);
	mpDangerText->SetFontSize(TEXT_FONT_SIZE);
}

// デストラクタ
CRewindUI::~CRewindUI()
{
	SAFE_DELETE(mpImg);
	SAFE_DELETE(mpDangerText);
}

// 更新
void CRewindUI::Update()
{
	mpImg->Update();
	if (mpDangerText->IsEnable())
	{
		mpDangerText->Update();
	}
}

// 描画
void CRewindUI::Render()
{
	mpImg->Render();
	if (mpDangerText->IsEnable())
	{
		mpDangerText->Render();
	}
}

// Dangerの文字の有効無効を設定
void CRewindUI::SetTextEnable(bool enable)
{
	mpDangerText->SetEnable(enable);
}

// 画像の色を設定
void CRewindUI::SetImgColor(CColor col)
{
	mpImg->SetColor(col);
	mpImg->SetAlpha(ALPHA);
}
