#include "CTextUI2D.h"
#include "CText.h"
#include <stdarg.h>

// �R���X�g���N�^
CTextUI2D::CTextUI2D()
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, ETaskPauseType::eGame)
{
	// ������`��p�̃N���X�𐶐�
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
	mpText->SetPos(Position());
	mpText->Update();
}

// �`��
void CTextUI2D::Render()
{
	mpText->Render();
}

// �`�悷�镶�����ݒ肷��
void CTextUI2D::ChangeStr(const char* format, ...)
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
}
