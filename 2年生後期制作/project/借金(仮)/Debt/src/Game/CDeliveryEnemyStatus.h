#pragma once
#include "CDeliveryStatusBase.h"

// �z�B�̓G�X�e�[�^�X�N���X
class CDeliveryEnemyStatus : public CDeliveryStatusBase
{
public:
	// �R���X�g���N�^
	CDeliveryEnemyStatus();
	// �f�X�g���N�^
	~CDeliveryEnemyStatus();

	// �󂵂���Ⴆ�链�_���擾
	int GetScore() const;

private:
	// �󂵂���Ⴆ�链�_
	int mScore;
};