#include "CDeliveryPlayerStatus.h"

#define MAX_HP 10						// �ő�HP
#define BASE_MOVE_SPEED 7.5f * 60.0f	// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x
#define ATTACK_POWER 1					// �U����
#define THROW_SPEED 7.5f * 60.0f * 2.0f	// �����鑬�x

// Hp�������̈ړ����x�̌����l
#define SPEED_DOWN 7.5f

// �R���X�g���N�^
CDeliveryPlayerStatus::CDeliveryPlayerStatus()
	: CDeliveryStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, ATTACK_POWER, THROW_SPEED)
{
}

// �f�X�g���N�^
CDeliveryPlayerStatus::~CDeliveryPlayerStatus()
{
}