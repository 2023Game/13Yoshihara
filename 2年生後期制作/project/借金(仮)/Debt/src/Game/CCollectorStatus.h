#pragma once
#include "CTrashStatusBase.h"

/*
�S�~�E���Q�[���̉�����̃X�e�[�^�X
�S�~�E���Q�[���̃X�e�[�^�X���N���X���p��
*/
class CCollectorStatus : public CTrashStatusBase
{
public:
	// �R���X�g���N�^
	CCollectorStatus(bool punisher);
	// �f�X�g���N�^
	~CCollectorStatus();

	// ���d�u���p���ǂ������擾����
	bool GetPunisher() const;
private:
	// ���d�u���p���ǂ���
	bool mIsPunisher;
};