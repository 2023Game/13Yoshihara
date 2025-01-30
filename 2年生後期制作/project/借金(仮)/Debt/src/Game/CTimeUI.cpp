#include "CTimeUI.h"
#include "CTextUI2D.h"

// ����UI�̃I�t�Z�b�g���W
#define TIME_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.38f,10.0f,0.0f)

// �R���X�g���N�^
CTimeUI::CTimeUI(int maxTime)
	: mTime(maxTime)
	, mElapsedTime(0.0f)
{
	mpTimeUI = new CTextUI2D();
}

// �f�X�g���N�^
CTimeUI::~CTimeUI()
{
}

// �X�V
void CTimeUI::Update()
{
	// �`�悷�镶�����ݒ�
	mpTimeUI->ChangeStr("�̂��肶����F%d\n", GetTime());
	// ���W��ݒ�
	mpTimeUI->Position(TIME_UI_OFFSET_POS);
	// ���Ԃ𐔂���
	Countdown();
}

// ���Ԃ𐔂���
void CTimeUI::Countdown()
{
	// �o�ߎ��Ԃ����Z
	mElapsedTime += Times::DeltaTime();
	// 1�b�ȏ�Ȃ�
	if (mElapsedTime >= 1.0f)
	{
		// 1�b�}�C�i�X
		mElapsedTime -= 1.0f;
		mTime--;
	}
}

// ���Ԃ��o�߂�����
bool CTimeUI::GetElapsedTime()
{
	return mTime <= 0;
}

// ���Ԃ��擾����
int CTimeUI::GetTime() const
{
	return mTime;
}
