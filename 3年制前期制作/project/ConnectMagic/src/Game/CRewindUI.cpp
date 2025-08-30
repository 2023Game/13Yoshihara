#include "CRewindUI.h"
#include "CImage.h"
#include "CTextUI2D.h"
#include "CGaugeUI2D.h"

// �摜�̃p�X
#define IMG_PATH "UI\\white.png"
// �摜�̃A���t�@
#define ALPHA 0.1f
// �������W
#define TEXT_POS CVector(0.0f,120.0f,0.0f)
// �����T�C�Y
#define TEXT_FONT_SIZE 128

// �R���X�g���N�^
CRewindUI::CRewindUI()
	: CTask(ETaskPriority::eSystem, 0, ETaskPauseType::eSystem, false, true)
{
	// �摜
	mpImg = new CImage(IMG_PATH, ETaskPriority::eSystem, 0, ETaskPauseType::eSystem, false, false);
	mpImg->SetColor(CColor::red);
	mpImg->SetAlpha(ALPHA);
	mpImg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// �e�L�X�g
	mpDangerText = new CTextUI2D(ETaskPauseType::eSystem, false);
	mpDangerText->SetStr("DANGER");
	mpDangerText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	mpDangerText->Position(TEXT_POS);
	mpDangerText->SetFontSize(TEXT_FONT_SIZE);
}

// �f�X�g���N�^
CRewindUI::~CRewindUI()
{
	SAFE_DELETE(mpImg);
	SAFE_DELETE(mpDangerText);
}

// �X�V
void CRewindUI::Update()
{
	mpImg->Update();
	if (mpDangerText->IsEnable())
	{
		mpDangerText->Update();
	}
}

// �`��
void CRewindUI::Render()
{
	mpImg->Render();
	if (mpDangerText->IsEnable())
	{
		mpDangerText->Render();
	}
}

// Danger�̕����̗L��������ݒ�
void CRewindUI::SetTextEnable(bool enable)
{
	mpDangerText->SetEnable(enable);
}

// �摜�̐F��ݒ�
void CRewindUI::SetImgColor(CColor col)
{
	mpImg->SetColor(col);
	mpImg->SetAlpha(ALPHA);
}
