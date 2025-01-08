#include "CTrashEnemyStatus.h"

#define MAX_HP 3						// �ő�HP
#define MOVE_SPEED 0.375f * 2.0f * 20.0f// �ړ����x
#define JUMP_SPEED 1.5f					// �W�����v���x
#define KNOCKBACK_SPEED 0.375f * 2.0f	// �m�b�N�o�b�N���x
#define ATTACK_POWER 1					// �U����
#define POWER 1							// �S�~�𗎂Ƃ��p���[

// �R���X�g���N�^
CTrashEnemyStatus::CTrashEnemyStatus()
	: CTrashStatusBase(MAX_HP, MOVE_SPEED, JUMP_SPEED,
		KNOCKBACK_SPEED, ATTACK_POWER, POWER)
{
}

// �f�X�g���N�^
CTrashEnemyStatus::~CTrashEnemyStatus()
{
}
