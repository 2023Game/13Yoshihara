#include "CTextUI2D.h"
#include "CFont.h"
#include <stdarg.h>
#include "Maths.h"

#define DEFAULT_FONT_SIZE 64

// �_�Ŏ���
#define TEXT_BLINK_TIME 1.0f
// �_�ŊԊu
#define TEXT_INTERVAL_TIME 1.0f

// �R���X�g���N�^
CTextUI2D::CTextUI2D(ETaskPauseType pauseType, bool addTaskList, char* fontPath)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, pauseType, false, addTaskList)
	, mIsBlink(false)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mpOwner(nullptr)
{
	// �t�H���g�f�[�^�𐶐�
	mpFont = new CFont(fontPath);
	mpFont->SetFontSize(DEFAULT_FONT_SIZE);
	mpFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpFont->SetLineLength(WINDOW_WIDTH);

	// ������`��p�̃N���X�𐶐�
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

// �f�X�g���N�^
CTextUI2D::~CTextUI2D()
{
	// ������ɍ폜���ꂽ���Ƃ�`����
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// �X�V
void CTextUI2D::Update()
{
	// �_�ł���Ȃ�_�ł��鏈�����Ă�
	if (mIsBlink)
	{
		Blink();
	}
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

// �����̐F��ݒ肷��
void CTextUI2D::SetFontColor(CColor color)
{
	mpText->SetColor(color);
}

// �A�E�g���C���̐F��ݒ肷��
void CTextUI2D::SetOutLineColor(CColor color)
{
	mpText->SetOutlineColor(color);
}

// �����̑����̊��ݒ�
void CTextUI2D::SetFontAligment(FTGL::TextAlignment aligment)
{
	// null�Ȃ珈�����Ȃ�
	if (mpFont == nullptr) return;
	mpFont->SetAlignment(aligment);
}

// �e��ݒ�
void CTextUI2D::SetShadow(bool enable, CColor color)
{
	mpText->SetEnableShadow(enable);
	mpText->SetShadowColor(color);
}

// �A�E�g���C����ݒ�
void CTextUI2D::SetOutline(bool enable, CColor color)
{
	mpText->SetEnableOutline(enable);
	mpText->SetOutlineColor(color);
}

// �_�ł��邩��ݒ�
void CTextUI2D::SetBlink(bool enable)
{
	mIsBlink = enable;
}

// �_�ł��鏈��
void CTextUI2D::Blink()
{
	// �ҋ@��Ԃ��X�e�b�v�Ǘ�
	switch (mStateStep)
	{
		// �X�e�b�v0�F�_�ő҂�
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
		// �X�e�b�v1�F�_��
	case 1:
		// �_��
		if (mElapsedTime < TEXT_BLINK_TIME)
		{
			// �T�C���J�[�u�œ_��
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

// �������ݒ�
void CTextUI2D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}
