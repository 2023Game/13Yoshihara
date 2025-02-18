#include "CToDoUI.h"
#include "CTextUI2D.h"
#include "CImage.h"

// �w�i�̃T�C�Y
#define BG_SIZE CVector2(WINDOW_WIDTH, WINDOW_HEIGHT * 0.17f)

// �e�L�X�g�̃I�t�Z�b�g���W
#define TEXT_OFFSET_POS CVector2(50.0f, 50.0f)

// �R���X�g���N�^
CToDoUI::CToDoUI()
	: CHomeUIBase()
	, mTexts({})
	, mNowTextNum(0)
{
	// �e�L�X�g�̍��W��ݒ�
	mpTextUI->Position(TEXT_OFFSET_POS);

	// �w�i�̃T�C�Y��ݒ�
	mpBackground->SetSize(BG_SIZE);
}

// �f�X�g���N�^
CToDoUI::~CToDoUI()
{
	mTexts.clear();
}

// �X�V
void CToDoUI::Update()
{
	// �`�悷�镶�����ݒ�
	mpTextUI->SetStr("��邱�ƁF" + mTexts[mNowTextNum]);

	CHomeUIBase::Update();
}

// �`��
void CToDoUI::Render()
{
	CHomeUIBase::Render();
}

// ��邱�Ƃ�\�����镶��������X�g�ɒǉ�
void CToDoUI::AddText(std::string text)
{
	mTexts.push_back(text);
}

// ��邱�Ƃ�\�����镶��������X�g����ݒ肷��
void CToDoUI::SetText(int num)
{
	// �T�C�Y�ȏ�̐����Ȃ珈�����Ȃ�
	if (mTexts.size() <= num) return;

	mNowTextNum = num;
}