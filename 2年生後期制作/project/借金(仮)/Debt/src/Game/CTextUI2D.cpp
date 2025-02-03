#include "CTextUI2D.h"
#include "CFont.h"
#include <stdarg.h>

// コンストラクタ
CTextUI2D::CTextUI2D(ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, pauseType, false, addTaskList)
{
	// タイトルロゴのフォントデータを生成
	mpFont = new CFont(fontPath);
	mpFont->SetFontSize(64);
	mpFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpFont->SetLineLength(WINDOW_WIDTH);

	// 文字列描画用のクラスを生成
	mpText = new CText
	(
		mpFont, 64,
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

// 文字の揃いの基準を設定
void CTextUI2D::SetFontAligment(FTGL::TextAlignment aligment)
{
	// nullなら処理しない
	if (mpFont == nullptr) return;
	mpFont->SetAlignment(aligment);
}
