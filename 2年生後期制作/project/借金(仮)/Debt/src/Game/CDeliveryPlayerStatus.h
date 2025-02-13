#pragma once
#include "CCharaStatusBase.h"

// �z�B�̃v���C���[�X�e�[�^�X�N���X
class CDeliveryPlayerStatus : public CCharaStatusBase
{
public:
	// �R���X�g���N�^
	CDeliveryPlayerStatus();
	// �f�X�g���N�^
	~CDeliveryPlayerStatus();

	// ��b�ړ����x���擾
	float GetBaseMoveSpeed() const override;
	// �����鑬�x���擾
	float GetThrowSpeed() const;
private:
	float mThrowSpeed;
};