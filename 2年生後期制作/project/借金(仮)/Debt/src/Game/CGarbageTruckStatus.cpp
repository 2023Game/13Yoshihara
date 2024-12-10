#include "CGarbageTruckStatus.h"

// �g��Ȃ��x�[�X�X�e�[�^�X�͒l��0
#define MAX_HP 3						// �ő�HP
#define MOVE_SPEED 0.375f * 5.0f		// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x
#define KNOCKBACK_SPEED 0.375f * 5.0f	// �m�b�N�o�b�N���x
#define POWER 1							// �S�~�𗎂Ƃ��p���[

#define BACK_TIME 10.0f		// �P�ނ܂ł̎���
#define COLLECT_RADIUS 5.0f	// ����͈͂̔��a
#define COLLECTORS 3		// ������̐l��

CGarbageTruckStatus::CGarbageTruckStatus()
{
	// ����͈͂̔��a��ݒ�
	mCollectRadius = COLLECT_RADIUS;
	// ������̐l����ݒ�
	SetCollectors(COLLECTORS);
}

CGarbageTruckStatus::~CGarbageTruckStatus()
{
}
// �P�ނ܂ł̎��Ԃ��擾
float CGarbageTruckStatus::GetBackTime()
{
	return mBackTime;
}

// �P�ނ܂ł̎��Ԃ�ݒ�
void CGarbageTruckStatus::SetBackTime(float backTime)
{
	mBackTime = backTime;
}

// �P�ނ܂ł̎��Ԃ��J�E���g
void CGarbageTruckStatus::CountBackTime()
{
	mBackTime -= Times::DeltaTime();
}

// ����͈͂̔��a���擾����
float CGarbageTruckStatus::GetCollectRadius()
{
	return mCollectRadius;
}

// ������̐l�����擾����
int CGarbageTruckStatus::GetCollectors()
{
	return mCollectors;
}

// ������̐l����ݒ肷��
void CGarbageTruckStatus::SetCollectors(int collectors)
{
	mCollectors = collectors;
}
