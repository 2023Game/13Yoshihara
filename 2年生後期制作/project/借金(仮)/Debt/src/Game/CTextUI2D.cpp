#include "CTextUI2D.h"
#include "CText.h"
#include <stdarg.h>

// コンストラクタ
CTextUI2D::CTextUI2D()
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, ETaskPauseType::eGame)
{
	// 文字列描画用のクラスを生成
	mpText = new CText
	(
		nullptr, 40,
		CVector::zero,
		CVector2(WINDOW_WIDTH,WINDOW_HEIGHT),
		CColor::white,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,false
	);
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
	mpText->SetPos(Position());
	mpText->Update();
}

// 描画
void CTextUI2D::Render()
{
	mpText->Render();
}

// 描画する文字列を設定する
void CTextUI2D::ChangeStr(const char* format, ...)
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
}
