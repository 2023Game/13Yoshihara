#include "CPlayerStatus.h"

#define MAX_HP 5				// �ő�̗�
#define MOVE_SPEED 2.0f * 60.0f	// �ړ����x
#define JUMP_SPEED 1.2f * 60.0f	// �W�����v��
#define ATTACK_POWER 0			// �U����


// �R���X�g���N�^
CPlayerStatus::CPlayerStatus()
	: CCharaStatusBase(MAX_HP, MOVE_SPEED, JUMP_SPEED, ATTACK_POWER)
{
}

// �f�X�g���N�^
CPlayerStatus::~CPlayerStatus()
{
}
