#include "CTextUI2D.h"
#include "CFont.h"
#include <stdarg.h>

// �R���X�g���N�^
CTextUI2D::CTextUI2D(ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, pauseType, false, addTaskList)
{
	// �^�C�g�����S�̃t�H���g�f�[�^�𐶐�
	mpFont = new CFont(fontPath);
	mpFont->SetFontSize(64);
	mpFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpFont->SetLineLength(WINDOW_WIDTH);

	// ������`��p�̃N���X�𐶐�
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

// �f�X�g���N�^
CTextUI2D::~CTextUI2D()
{
}

// �X�V
void CTextUI2D::Update()
{
	// �`�悷�镶�����ݒ�
	mpText->SetText(mStr.c_str());
	// ���W��ݒ�
	mpText->SetPos(Position().X(),Position().Y());
	mpText->Update();
}

// �`��
void CTextUI2D::Render()
{
	mpText->Render();
}

// �`�悷�镶�����ݒ肷��
void CTextUI2D::ChangeToStr(const char* format, ...)
{
	char buf[512];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);
	SetStr(buf);
}

// �`�悷�镶�����ݒ肷��
void CTextUI2D::SetStr(std::string str)
{
	mStr = str;
}

// �����̃T�C�Y��ݒ肷��
void CTextUI2D::SetFontSize(int fontSize)
{
	mpText->SetFontSize(fontSize);
	// null�Ȃ珈�����Ȃ�
	if (mpFont == nullptr) return;
	mpFont->SetFontSize(fontSize);
}

// �����̑����̊��ݒ�
void CTextUI2D::SetFontAligment(FTGL::TextAlignment aligment)
{
	// null�Ȃ珈�����Ȃ�
	if (mpFont == nullptr) return;
	mpFont->SetAlignment(aligment);
}
