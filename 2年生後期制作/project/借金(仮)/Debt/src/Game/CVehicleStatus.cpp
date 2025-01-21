#include "CVehicleStatus.h"

// �g��Ȃ��x�[�X�X�e�[�^�X�͒l��0
#define MAX_HP 0								// �ő�HP
#define BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// �ړ����x
#define JUMP_SPEED 0.0f							// �W�����v���x
#define KNOCKBACK_SPEED 0.375f * 5.0f			// �m�b�N�o�b�N���x
#define ATTACK_POWER 1							// �U����
#define POWER 1									// �S�~�𗎂Ƃ��p���[

#define DEATH_WAIT_TIME 2.0f	// ���ł܂ł̎���

// �R���X�g���N�^
CVehicleStatus::CVehicleStatus()
	: CTrashStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED,
		KNOCKBACK_SPEED, ATTACK_POWER, POWER)
	, mDeleteTime(DEATH_WAIT_TIME)
{
}

/*
�R���X�g���N�^
���̃N���X���p�������L�����̃X�e�[�^�X�̐ݒ�p
*/
CVehicleStatus::CVehicleStatus(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockback, int attackPower, int power)
	: CTrashStatusBase(maxHp, baseMoveSpeed, jumpSpeed,
		knockback, attackPower, power)
	, mDeleteTime(DEATH_WAIT_TIME)
{
}

CVehicleStatus::~CVehicleStatus()
{
}

// ���ł܂ł̎��Ԃ��擾����
float CVehicleStatus::GetDeleteTime() const
{
	return mDeleteTime;
}

// ���ł܂ł̎��Ԃ������l�ɐݒ肷��
void CVehicleStatus::SetDeleteTime()
{
	mDeleteTime = DEATH_WAIT_TIME;
}

// ���ł܂ł̎��Ԃ��o�߂������ǂ���
bool CVehicleStatus::IsElapsedDeleteTime() const
{
	// ���ł܂ł̎��Ԃ�0���傫���Ȃ�o�߂��Ă��Ȃ�
	if (mDeleteTime > 0.0f)
	{
		return false;
	}
	// 0�ȉ��Ȃ�o�߂���
	else
	{
		return true;
	}
}

// ���ł܂ł̎��Ԃ��J�E���g�_�E��
void CVehicleStatus::CountDeleteTime()
{
	// 0���傫���Ȃ猸�Z
	if (mDeleteTime > 0.0f)
	{
		mDeleteTime -= Times::DeltaTime();
	}
}


