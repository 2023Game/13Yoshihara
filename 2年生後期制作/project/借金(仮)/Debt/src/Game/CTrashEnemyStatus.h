#pragma once
// �S�~�E���Q�[���̃X�e�[�^�X���N���X���C���N���[�h
#include "CTrashStatusBase.h"

/*
�S�~�E���Q�[���̓G�X�e�[�^�X
�S�~�E���Q�[���̃X�e�[�^�X���N���X���p��
*/
class CTrashEnemyStatus : public CTrashStatusBase
{
public:
	// �R���X�g���N�^
	CTrashEnemyStatus(bool punisher);
	// �f�X�g���N�^
	~CTrashEnemyStatus();

	// �N���e�B�J���m�����擾
	int GetCriticalChance();
	// �N���e�B�J���m����ݒ�
	void SetCriticalChance(int criticalChance);

	// �S�~�܂̐��̏����l���擾����
	int GetDefaultBagNum() const;
	// �S�[���h�S�~�܂̐��̏����l���擾����
	int GetDefaultGoldBagNum() const;

private:
	int mCriticalChance;	// �N���e�B�J���m��
	bool mIsPunisher;		// ���d�u���p���ǂ���
	int mDefaultBagNum;		// �S�~�܂̐��̏����l
	int mDefaultGoldBagNum;	// �S�[���h�S�~�܂̐��̏����l
};