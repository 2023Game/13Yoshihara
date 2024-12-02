#include "CCarStatus.h"

// �g��Ȃ��x�[�X�X�e�[�^�X�͒l��0
#define MAX_HP 0						// �ő�HP
#define MOVE_SPEED 0.375f * 5.0f		// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x
#define KNOCKBACK_SPEED 0.375f * 5.0f	// �m�b�N�o�b�N���x
#define POWER 1							// �S�~�𗎂Ƃ��p���[

#define POP_TIME 10.0f		// �o���܂ł̎���
#define DELETE_TIME 2.0f	// ���ł܂ł̎���

CCarStatus::CCarStatus()
	: CTrashStatusBase()
{
	// �x�[�X�X�e�[�^�X��ݒ�
	mStatus =
	{
		MAX_HP,
		MOVE_SPEED,
		JUMP_SPEED,
		KNOCKBACK_SPEED,
		POWER,
	};

	// �o���܂ł̎���
	mPopTime = POP_TIME;
	// ���ł܂ł̎���
	mDeleteTime = DELETE_TIME;
}

CCarStatus::~CCarStatus()
{
}
