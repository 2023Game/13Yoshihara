#include "CGarbageTruckStatus.h"

// �g��Ȃ��x�[�X�X�e�[�^�X�͒l��0
#define MAX_HP 3						// �ő�HP
#define MOVE_SPEED 0.375f * 5.0f		// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x
#define KNOCKBACK_SPEED 0.375f * 5.0f	// �m�b�N�o�b�N���x
#define ATTACK_POWER 1					// �U����
#define POWER 1							// �S�~�𗎂Ƃ��p���[

#define BACK_TIME 10.0f		// �P�ނ܂ł̎���
#define COLLECT_RADIUS 5.0f	// ����͈͂̔��a
#define COLLECTORS 3		// ������̐l��

CGarbageTruckStatus::CGarbageTruckStatus()
	: CVehicleStatus(MAX_HP, MOVE_SPEED, JUMP_SPEED,
		KNOCKBACK_SPEED, ATTACK_POWER, POWER)
	, mBackTime(BACK_TIME)
	, mCollectRadius(COLLECT_RADIUS)
	, mCollectors(COLLECTORS)
{
}

CGarbageTruckStatus::~CGarbageTruckStatus()
{
}
// �P�ނ܂ł̎��Ԃ��擾
float CGarbageTruckStatus::GetBackTime() const
{
	return mBackTime;
}

// �P�ނ܂ł̎��Ԃ������l�ɐݒ�
void CGarbageTruckStatus::SetBackTime()
{
	mBackTime = BACK_TIME;
}

// �P�ނ܂ł̎��Ԃ��o�߂������ǂ���
bool CGarbageTruckStatus::IsElapsedBackTime() const
{
	// �P�ނ܂ł̎��Ԃ�0���傫���Ȃ�o�߂��Ă��Ȃ�
	if (mBackTime > 0.0f)
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
void CGarbageTruckStatus::CountBackTime()
{
	mBackTime -= Times::DeltaTime();
}

// ����͈͂̔��a���擾����
float CGarbageTruckStatus::GetCollectRadius() const
{
	return mCollectRadius;
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


