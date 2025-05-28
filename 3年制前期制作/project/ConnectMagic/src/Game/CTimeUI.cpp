#include "CTimeUI.h"
#include "CTextUI2D.h"
#include "CTaskManager.h"

// ����UI�̃I�t�Z�b�g���W
#define TIME_UI_OFFSET_POS CVector(WINDOW_WIDTH * 0.35f,10.0f,0.0f)

// �R���X�g���N�^
CTimeUI::CTimeUI(int maxTime)
	: mTimeLocation(maxTime)
	, mElapsedTime(0.0f)
{
	// ����UI�𐶐�
	mpTimeUI = new CTextUI2D();
	// �����̑����̊��ݒ�
	mpTimeUI->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// ���W��ݒ�
	mpTimeUI->Position(TIME_UI_OFFSET_POS);
}

// �f�X�g���N�^
CTimeUI::~CTimeUI()
{
}

// �X�V
void CTimeUI::Update()
{
	// �`�悷�镶�����ݒ�
	mpTimeUI->ChangeToStr("Time�F % d\n", GetTime());
	// ���Ԃ𐔂���
	Countdown();
}

// ���Ԃ𐔂���
void CTimeUI::Countdown()
{
	CTaskManager* taskMgr=CTaskManager::Instance();
	// �|�[�Y����Ă��Ȃ��ꍇ
	if (!taskMgr->IsPaused())
	{
		// �o�ߎ��Ԃ����Z
		mElapsedTime += Times::DeltaTime();
	}
	// 1�b�ȏ�Ȃ�
	if (mElapsedTime >= 1.0f)
	{
		// 1�b�}�C�i�X
		mElapsedTime -= 1.0f;
		mTimeLocation--;
	}
}

// ���Ԃ��o�߂�����
bool CTimeUI::GetElapsedTime()
{
	return mTimeLocation <= 0;
}

// ���Ԃ��擾����
int CTimeUI::GetTime() const
{
	return mTimeLocation;
}
