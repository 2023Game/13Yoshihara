#include "CDeliveryPlayerStatus.h"

#define MAX_HP 10						// �ő�HP
#define BASE_MOVE_SPEED 7.5f * 60.0f	// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x
#define ATTACK_POWER 1					// �U����

// �R���X�g���N�^
CDeliveryPlayerStatus::CDeliveryPlayerStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, ATTACK_POWER)
{
}

// �f�X�g���N�^
CDeliveryPlayerStatus::~CDeliveryPlayerStatus()
{
}
