#include "CTrashPlayerStatus.h"

#define MAX_HP 10								// �ő�HP
#define BASE_MOVE_SPEED 0.75f * 60.0f			// �ړ����x
#define JUMP_SPEED 1.5f							// �W�����v���x
#define KNOCKBACK_SPEED 0.75f * 60.0f * 2.0f	// �m�b�N�o�b�N���x
#define ATTACK_POWER 1							// �U����
#define POWER 1									// �S�~�𗎂Ƃ��p���[
#define CRITICAL_CHANCE 10						// �N���e�B�J���m��

// �R���X�g���N�^
CTrashPlayerStatus::CTrashPlayerStatus()
	: CTrashStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED, 
		KNOCKBACK_SPEED, ATTACK_POWER, POWER)
	, mCriticalChance(CRITICAL_CHANCE)
{

}

// �f�X�g���N�^
CTrashPlayerStatus::~CTrashPlayerStatus()
{
}

// �N���e�B�J���m�����擾
int CTrashPlayerStatus::GetCriticalChance()
{
	return mCriticalChance;
}


