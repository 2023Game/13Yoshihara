#include "CGarbageTruckStatus.h"

// �g��Ȃ��x�[�X�X�e�[�^�X�͒l��0
#define MAX_HP 3								// �ő�HP
#define BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// �ړ����x
#define JUMP_SPEED 0.0f							// �W�����v���x
#define KNOCKBACK_SPEED 0.375f * 5.0f			// �m�b�N�o�b�N���x
#define ATTACK_POWER 1							// �U����
#define POWER 1									// �S�~�𗎂Ƃ��p���[

#define WITHDRAW_TIME 10.0f		// �P�ނ܂ł̎���
#define COLLECTORS 3			// ������̐l��

CGarbageTruckStatus::CGarbageTruckStatus()
	: CVehicleStatus(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED,
		KNOCKBACK_SPEED, ATTACK_POWER, POWER)
	, mWithdrawTime(WITHDRAW_TIME)
	, mCollectors(COLLECTORS)
{
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
	mWithdrawTime = WITHDRAW_TIME;
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
	mCollectors = COLLECTORS;
}

// ������̐l����1���炷
void CGarbageTruckStatus::DecreaseCollectors()
{
	mCollectors--;
}


