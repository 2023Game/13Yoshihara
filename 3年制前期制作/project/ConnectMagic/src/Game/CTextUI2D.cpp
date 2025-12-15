#include "CTextUI2D.h"
#include "CFont.h"
#include <stdarg.h>
#include "Maths.h"

constexpr int DEFAULT_FONT_SIZE = 64;

// 点滅時間
constexpr float TEXT_BLINK_TIME = 1.0f;
// 点滅間隔
constexpr float TEXT_INTERVAL_TIME = 1.0f;

// コンストラクタ
CTextUI2D::CTextUI2D(ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, pauseType, false, addTaskList)
	, mIsBlink(false)
	, mStateStep(0)
	, mElapsedTime(0.0f)
{
	// フォントデータを生成
	mpFont = new CFont(fontPath);
	mpFont->SetFontSize(DEFAULT_FONT_SIZE);
	mpFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpFont->SetLineLength(WINDOW_WIDTH);

	// 文字列描画用のクラスを生成
	mpText = new CText
	(
		mpFont, DEFAULT_FONT_SIZE,
		CVector::zero,
		CVector2(WINDOW_WIDTH,WINDOW_HEIGHT),
		CColor(0.1f, 0.1f, 0.1f),
		ETaskPriority::eUI,
		0,
		pauseType,
		false,false
	);
	mpText->SetEnableOutline(true);
	mpText->SetOutlineColor(CColor(0.9f, 0.9f, 0.9f));
}

// デストラクタ
CTextUI2D::~CTextUI2D()
{
}

// 更新
void CTextUI2D::Update()
{
	// 点滅するなら点滅する処理を呼ぶ
	if (mIsBlink)
	{
		Blink();
	}
	// 描画する文字列を設定
	mpText->SetText(mStr.c_str());
	// 座標を設定
	mpText->SetPos(Position().X(),Position().Y());
	mpText->Update();
}

// 描画
void CTextUI2D::Render()
{
	mpText->Render();
}

// 描画する文字列を設定する
void CTextUI2D::ChangeToStr(const char* format, ...)
{
	char buf[512];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);
	SetStr(buf);
}

// 描画する文字列を設定する
void CTextUI2D::SetStr(std::string str)
{
	mStr = str;
}

// 文字のサイズを設定する
void CTextUI2D::SetFontSize(int fontSize)
{
	mpText->SetFontSize(fontSize);
	// nullなら処理しない
	if (mpFont == nullptr) return;
	mpFont->SetFontSize(fontSize);
}

// 文字の色を設定する
void CTextUI2D::SetFontColor(CColor color)
{
	mpText->SetColor(color);
}

// アウトラインの色を設定する
void CTextUI2D::SetOutLineColor(CColor color)
{
	mpText->SetOutlineColor(color);
}

// 文字の揃いの基準を設定
void CTextUI2D::SetFontAligment(FTGL::TextAlignment aligment)
{
	// nullなら処理しない
	if (mpFont == nullptr) return;
	mpFont->SetAlignment(aligment);
}

// 影を設定
void CTextUI2D::SetShadow(bool enable, CColor color)
{
	mpText->SetEnableShadow(enable);
	mpText->SetShadowColor(color);
}

// アウトラインを設定
void CTextUI2D::SetOutline(bool enable, CColor color)
{
	mpText->SetEnableOutline(enable);
	mpText->SetOutlineColor(color);
}

// 点滅するかを設定
void CTextUI2D::SetBlink(bool enable)
{
	mIsBlink = enable;
}

// 点滅する処理
void CTextUI2D::Blink()
{
	// 待機状態をステップ管理
	switch (mStateStep)
	{
		// ステップ0：点滅待ち
	case 0:
		mpText->SetAlpha(1.0f);
		if (mElapsedTime < TEXT_INTERVAL_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			mElapsedTime = 0.0f;
			mStateStep = 1;
		}
		break;
		// ステップ1：点滅
	case 1:
		// 点滅
		if (mElapsedTime < TEXT_BLINK_TIME)
		{
			// サインカーブで点滅
			float per = sinf(M_PI * mElapsedTime / TEXT_BLINK_TIME);
			mpText->SetAlpha(1.0f - per);
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			mElapsedTime = 0.0f;
			mpText->SetAlpha(1.0f);
			mStateStep = 0;
		}
		break;
	}
}
