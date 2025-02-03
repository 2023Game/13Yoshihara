#include "CToDoUI.h"
#include "CTextUI2D.h"

// �R���X�g���N�^
CToDoUI::CToDoUI()
	: mTexts({})
	, mNowTextNum(0)
{
	// UI�𐶐�
	mpToDoUI = new CTextUI2D(ETaskPauseType::eMenu, true, nullptr);
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
	mpToDoUI->SetStr("��邱�ƁF" + mTexts[mNowTextNum]);
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
