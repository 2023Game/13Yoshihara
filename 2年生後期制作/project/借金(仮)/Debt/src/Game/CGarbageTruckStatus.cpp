#include "CGarbageTruckStatus.h"

// �g��Ȃ��x�[�X�X�e�[�^�X�͒l��0
// �ʏ�̃S�~���W�Ԃ̃X�e�[�^�X
#define NORMAL_MAX_HP 3									// �ő�HP
#define NORMAL_BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// �ړ����x
#define NORMAL_JUMP_SPEED 0.0f							// �W�����v���x
#define NORMAL_KNOCKBACK_SPEED 0.375f * 5.0f			// �m�b�N�o�b�N���x
#define NORMAL_ATTACK_POWER 1							// �U����
#define NORMAL_POWER 1									// �S�~�𗎂Ƃ��p���[
#define NORMAL_WITHDRAW_TIME 10.0f						// �P�ނ܂ł̎���
#define NORMAL_COLLECTORS 3								// ������̐l��
// ���d�u���̃S�~���W�Ԃ̃X�e�[�^�X
#define PUNISHER_MAX_HP 3								// �ő�HP
#define PUNISHER_BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// �ړ����x
#define PUNISHER_JUMP_SPEED 0.0f						// �W�����v���x
#define PUNISHER_KNOCKBACK_SPEED 0.375f * 5.0f			// �m�b�N�o�b�N���x
#define PUNISHER_ATTACK_POWER 1							// �U����
#define PUNISHER_POWER 1								// �S�~�𗎂Ƃ��p���[
#define PUNISHER_WITHDRAW_TIME 10.0f					// �P�ނ܂ł̎���
#define PUNISHER_COLLECTORS 3							// ������̐l��

CGarbageTruckStatus::CGarbageTruckStatus(bool punisher)
	: mIsPunisher(punisher)
{
	// ���d�u���p�Ȃ�
	if (punisher)
	{
		// ���d�u���p�̃X�e�[�^�X
		CVehicleStatus(PUNISHER_MAX_HP, PUNISHER_BASE_MOVE_SPEED, PUNISHER_JUMP_SPEED,
			PUNISHER_KNOCKBACK_SPEED, PUNISHER_ATTACK_POWER, PUNISHER_POWER);
		mWithdrawTime = PUNISHER_WITHDRAW_TIME;
		mCollectors = PUNISHER_COLLECTORS;
	}
	// �ʏ�p�Ȃ�
	else
	{
		// �ʏ�̃X�e�[�^�X
		CVehicleStatus(NORMAL_MAX_HP, NORMAL_BASE_MOVE_SPEED, NORMAL_JUMP_SPEED,
			NORMAL_KNOCKBACK_SPEED, NORMAL_ATTACK_POWER, NORMAL_POWER);
		mWithdrawTime = NORMAL_WITHDRAW_TIME;
		mCollectors = NORMAL_COLLECTORS;
	}
}

CGarbageTruckStatus::~CGarbageTruckStatus()
{
}
// �P�ނ܂ł̎��Ԃ��擾
float CGarbageTruckStatus::GetBackTime() const
{
	return mWithdrawTime;
}

// �P�ނ܂ł̎��Ԃ������l�ɐݒ�
void CGarbageTruckStatus::SetWithdrawTime()
{
	// ���d�u���p
	if (mIsPunisher)
	{
		mWithdrawTime = PUNISHER_WITHDRAW_TIME;
	}
	// �ʏ�p
	else
	{
		mWithdrawTime = NORMAL_WITHDRAW_TIME;
	}
}

// �P�ނ܂ł̎��Ԃ��o�߂������ǂ���
bool CGarbageTruckStatus::IsElapsedWithdrawTime() const
{
	// �P�ނ܂ł̎��Ԃ�0���傫���Ȃ�o�߂��Ă��Ȃ�
	if (mWithdrawTime > 0.0f)
	{
		return false;
	}
	// 0�ȉ��Ȃ�o�߂���
	else
	{
		return true;
	}
}

// �P�ނ܂ł̎��Ԃ��J�E���g�_�E��
void CGarbageTruckStatus::CountWithdrawTime()
{
	mWithdrawTime -= Times::DeltaTime();
}

// ������̐l�����擾����
int CGarbageTruckStatus::GetCollectors() const
{
	return mCollectors;
}

// ������̐l���������l�ɐݒ肷��
void CGarbageTruckStatus::SetCollectors()
{
	// ���d�u���p
	if (mIsPunisher)
	{
		mCollectors = PUNISHER_COLLECTORS;
	}
	// �ʏ�p
	else
	{
		mCollectors = NORMAL_COLLECTORS;
	}
}

// ������̐l����1���炷
void CGarbageTruckStatus::DecreaseCollectors()
{
	mCollectors--;
}


