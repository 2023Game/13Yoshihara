#pragma once
// �S�~�E���Q�[���̃X�e�[�^�X���N���X���C���N���[�h
#include "CTrashStatusBase.h"

/*
�S�~�E���Q�[���̃v���C���[�X�e�[�^�X
�S�~�E���Q�[���̃X�e�[�^�X���N���X���p��
*/
class CTrashPlayerStatus : public CTrashStatusBase
{
public:
	// �R���X�g���N�^
	CTrashPlayerStatus();
	// �f�X�g���N�^
	~CTrashPlayerStatus();

	// �N���e�B�J���m�����擾
	int GetCriticalChance();

private:
	// �N���e�B�J���m��
	int mCriticalChance;
};