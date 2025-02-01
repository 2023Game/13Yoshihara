#include "CGarbageTruckStatus.h"

// �g��Ȃ��x�[�X�X�e�[�^�X�͒l��0
// �ʏ�̃S�~���W�Ԃ̃X�e�[�^�X
#define NORMAL_MAX_HP 30								// �ő�HP
#define NORMAL_BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// �ړ����x
#define NORMAL_JUMP_SPEED 0.0f							// �W�����v���x
#define NORMAL_KNOCKBACK_SPEED 0.375f * 5.0f			// �m�b�N�o�b�N���x
#define NORMAL_ATTACK_POWER 1							// �U����
#define NORMAL_POWER 2									// �S�~�𗎂Ƃ��p���[
#define NORMAL_RETURN_TIME 20.0f						// �P�ނ܂ł̎���
#define NORMAL_COLLECTORS 1								// ������̐l��
#define NORMAL_DEFAULT_BAG_NUM 5						// �S�~�܂̐��̏����l
#define NORMAL_DEFAULT_GOLD_BAG_NUM 5					// �S�[���h�S�~�܂̐��̏����l
// ���d�u���̃S�~���W�Ԃ̃X�e�[�^�X
#define PUNISHER_MAX_HP 100								// �ő�HP
#define PUNISHER_BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// �ړ����x
#define PUNISHER_JUMP_SPEED 0.0f						// �W�����v���x
#define PUNISHER_KNOCKBACK_SPEED 0.375f * 5.0f			// �m�b�N�o�b�N���x
#define PUNISHER_ATTACK_POWER 1							// �U����
#define PUNISHER_POWER 2								// �S�~�𗎂Ƃ��p���[
#define PUNISHER_RETURN_TIME 1.0f						// �P�ނ܂ł̎���
#define PUNISHER_COLLECTORS 1							// ������̐l��
#define PUNISHER_DEFAULT_BAG_NUM 0						// �S�~�܂̐��̏����l
#define PUNISHER_DEFAULT_GOLD_BAG_NUM 10					// �S�[���h�S�~�܂̐��̏����l

CGarbageTruckStatus::CGarbageTruckStatus(bool punisher)
	: CVehicleStatus(NORMAL_MAX_HP, NORMAL_BASE_MOVE_SPEED, NORMAL_JUMP_SPEED,
		NORMAL_KNOCKBACK_SPEED, NORMAL_ATTACK_POWER, NORMAL_POWER)
	, mIsPunisher(punisher)
	, mReturnTime(NORMAL_RETURN_TIME)
	, mCollectorsNum(NORMAL_COLLECTORS)
	, mDefaultBagNum(NORMAL_DEFAULT_BAG_NUM)
	, mDefaultGoldBagNum(NORMAL_DEFAULT_GOLD_BAG_NUM)
{
	// ���d�u���p�Ȃ�
	if (punisher)
	{
		// ���d�u���p�̃X�e�[�^�X�ɕύX
		CVehicleStatus(PUNISHER_MAX_HP, PUNISHER_BASE_MOVE_SPEED, PUNISHER_JUMP_SPEED,
			PUNISHER_KNOCKBACK_SPEED, PUNISHER_ATTACK_POWER, PUNISHER_POWER);
		mReturnTime = PUNISHER_RETURN_TIME;
		mCollectorsNum = PUNISHER_COLLECTORS;
		mDefaultBagNum = PUNISHER_DEFAULT_BAG_NUM;
		mDefaultGoldBagNum = PUNISHER_DEFAULT_GOLD_BAG_NUM;
	}
}

CGarbageTruckStatus::~CGarbageTruckStatus()
{
}

// �P�ނ܂ł̎��Ԃ��擾
float CGarbageTruckStatus::GetReturnTime() const
{
	return mReturnTime;
}

// �P�ނ܂ł̎��Ԃ������l�ɐݒ�
void CGarbageTruckStatus::SetReturnTime()
{
	// ���d�u���p
	if (mIsPunisher)
	{
		mReturnTime = PUNISHER_RETURN_TIME;
	}
	// �ʏ�p
	else
	{
		mReturnTime = NORMAL_RETURN_TIME;
	}
}

// �P�ނ܂ł̎��Ԃ��o�߂������ǂ���
bool CGarbageTruckStatus::IsElapsedReturnTime() const
{
	// 0�ȉ��Ȃ�o�߂��Ă���
	return mReturnTime <= 0.0f;
}

// �P�ނ܂ł̎��Ԃ��J�E���g�_�E��
void CGarbageTruckStatus::CountReturnTime()
{
	mReturnTime -= Times::DeltaTime();
}

// ������̐l�����擾����
int CGarbageTruckStatus::GetCollectorsNum() const
{
	return mCollectorsNum;
}

// ������̐l���������l�ɐݒ肷��
void CGarbageTruckStatus::SetCollectorsNum()
{
	// ���d�u���p
	if (mIsPunisher)
	{
		mCollectorsNum = PUNISHER_COLLECTORS;
	}
	// �ʏ�p
	else
	{
		mCollectorsNum = NORMAL_COLLECTORS;
	}
}

// ������̐l����1���炷
void CGarbageTruckStatus::DecreaseCollectors()
{
	mCollectorsNum--;
}

// �������玝���Ă���S�~�܂̐����擾����
int CGarbageTruckStatus::GetDefaultBagNum() const
{
	return mDefaultBagNum;
}

// �������玝���Ă���S�[���h�S�~�܂̐����擾����
int CGarbageTruckStatus::GetDefaultGoldBagNum() const
{
	return mDefaultGoldBagNum;
}

// ���d�u���p���ǂ������擾
bool CGarbageTruckStatus::GetPunisher() const
{
	return mIsPunisher;
}


