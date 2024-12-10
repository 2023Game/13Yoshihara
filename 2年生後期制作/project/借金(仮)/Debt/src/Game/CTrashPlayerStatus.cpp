#include "CTrashPlayerStatus.h"

#define MAX_HP 3						// �ő�HP
#define MOVE_SPEED 0.375f * 2.0f		// �ړ����x
#define JUMP_SPEED 1.5f					// �W�����v���x
#define KNOCKBACK_SPEED 0.375f * 2.0f	// �m�b�N�o�b�N���x
#define POWER 1							// �S�~�𗎂Ƃ��p���[

CTrashPlayerStatus::CTrashPlayerStatus()
	: CTrashStatusBase()
{
	// �x�[�X�X�e�[�^�X��ݒ�
	SetHp(MAX_HP);
	SetBaseMoveSpeed(MOVE_SPEED);
	SetJumpSpeed(JUMP_SPEED);
	SetKnockback(KNOCKBACK_SPEED);
	SetPower(POWER);
}

CTrashPlayerStatus::~CTrashPlayerStatus()
{
}


