#include "CCollectorStatus.h"

// �ʏ�p�̉�����X�e�[�^�X
#define NORMAL_MAX_HP 1									// �ő�HP
#define NORMAL_BASE_MOVE_SPEED 0.375f * 2.0f * 60.0f	// �ړ����x
#define NORMAL_JUMP_SPEED 0.2f							// �W�����v���x
#define NORMAL_KNOCKBACK_SPEED 0.375f * 2.0f			// �m�b�N�o�b�N���x
#define NORMAL_ATTACK_POWER 0							// �U����
#define NORMAL_POWER 1									// �S�~�𗎂Ƃ��p���[
// ���d�u���p�̉�����X�e�[�^�X
#define PUNISHER_MAX_HP 1								// �ő�HP
#define PUNISHER_BASE_MOVE_SPEED 0.375f * 2.0f * 60.0f	// �ړ����x
#define PUNISHER_JUMP_SPEED 0.2f						// �W�����v���x
#define PUNISHER_KNOCKBACK_SPEED 0.375f * 2.0f			// �m�b�N�o�b�N���x
#define PUNISHER_ATTACK_POWER 0							// �U����
#define PUNISHER_POWER 2								// �S�~�𗎂Ƃ��p���[

// �R���X�g���N�^
CCollectorStatus::CCollectorStatus(bool punisher)
	: CTrashStatusBase(NORMAL_MAX_HP, NORMAL_BASE_MOVE_SPEED, NORMAL_JUMP_SPEED,
		NORMAL_KNOCKBACK_SPEED, NORMAL_ATTACK_POWER, NORMAL_POWER)
	, mIsPunisher(punisher)
{
	// ���d�u���p�Ȃ�
	if (punisher)
	{
		// ���d�u���p�̃X�e�[�^�X�ɕύX����
		SetAllStatus(PUNISHER_MAX_HP, PUNISHER_BASE_MOVE_SPEED, PUNISHER_JUMP_SPEED,
			PUNISHER_KNOCKBACK_SPEED, PUNISHER_ATTACK_POWER, PUNISHER_POWER);
	}
}

// �f�X�g���N�^
CCollectorStatus::~CCollectorStatus()
{
}

// ���d�u���p���ǂ������擾����
bool CCollectorStatus::GetPunisher() const
{
	return mIsPunisher;
}
