#include "CDeliveryStatusBase.h"

// Hp�������̈ړ����x�̌����l
#define SPEED_DOWN 7.5f

// �R���X�g���N�^
CDeliveryStatusBase::CDeliveryStatusBase(int maxHp, float moveSpeed,
	float jumpSpeed, int attackPower, float throwSpeed)
	: CCharaStatusBase(maxHp, moveSpeed,
		jumpSpeed, attackPower)
	, mThrowSpeed(throwSpeed)
{
}

// �f�X�g���N�^
CDeliveryStatusBase::~CDeliveryStatusBase()
{
}

// ��b�ړ����x���擾
float CDeliveryStatusBase::GetBaseMoveSpeed() const
{
	// ��b�ړ����x���擾
	float baseMoveSpeed = CCharaStatusBase::GetBaseMoveSpeed();
	// �������̌����l�~HP�A�������Ĉړ����x���v�Z
	float moveSpeed = baseMoveSpeed - SPEED_DOWN * GetHp();
	// 0�ȉ��Ȃ�0�ɂ���
	if (moveSpeed <= 0.0f) moveSpeed = 0.0f;
	return moveSpeed;
}

// �����鑬�x���擾
float CDeliveryStatusBase::GetThrowSpeed() const
{
	return mThrowSpeed;
}
