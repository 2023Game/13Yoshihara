#pragma once
#include "CTask.h"

class CImage;
class CTextUI2D;
class CSound;

// �S�~�E���Q�[���̌x���\���pUI
class CTrashWarningUI : public CTask
{
public:
	// �R���X�g���N�^
	CTrashWarningUI();
	// �f�X�g���N�^
	~CTrashWarningUI();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	// �x���\���p
	CImage* mpWarningImg;

	int mStateStep;
	float mElapsedTime; // �o�ߎ���

	CSound* mpAlarmSE;	// �x����
	float mIntervalTime;	// �Đ��Ԋu�̎���
};