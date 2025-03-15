#pragma once
#include "CCharaStatusBase.h"

// �z�B�̊��X�e�[�^�X�N���X
class CDeliveryStatusBase : public CCharaStatusBase
{
public:
	// �R���X�g���N�^
	CDeliveryStatusBase(int maxHp, float moveSpeed,
		float jumpSpeed, int attackPower, float throwSpeed);
	// �f�X�g���N�^
	~CDeliveryStatusBase();

	// ��b�ړ����x���擾
	float GetBaseMoveSpeed() const override;
	// �����鑬�x���擾
	float GetThrowSpeed() const;

private:
	// �����鑬�x
	float mThrowSpeed;
};