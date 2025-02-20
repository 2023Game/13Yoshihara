#include "CTrashEnemyStatus.h"

// �ʏ�p�̃S�~�E���G�X�e�[�^�X
#define NORMAL_MAX_HP 10							// �ő�HP
#define NORMAL_BASE_MOVE_SPEED 0.75f * 60.0f		// �ړ����x
#define NORMAL_JUMP_SPEED 1.5f						// �W�����v���x
#define NORMAL_KNOCKBACK_SPEED 0.75f * 60.0f * 2.5f	// �m�b�N�o�b�N���x
#define NORMAL_ATTACK_POWER 1						// �U����
#define NORMAL_POWER 1								// �S�~�𗎂Ƃ��p���[
#define NORMAL_CRITICAL_CHANCE 10					// �N���e�B�J���m��
// ���d�u���p�̃S�~�E���G�X�e�[�^�X
#define PUNISHER_MAX_HP 20								// �ő�HP
#define PUNISHER_BASE_MOVE_SPEED 0.75f * 60.0f * 2.5f	// �ړ����x
#define PUNISHER_JUMP_SPEED 1.5f						// �W�����v���x
#define PUNISHER_KNOCKBACK_SPEED 0.75f * 60.0f * 2.5f	// �m�b�N�o�b�N���x
#define PUNISHER_ATTACK_POWER 2							// �U����
#define PUNISHER_POWER 2								// �S�~�𗎂Ƃ��p���[
#define PUNISHER_CRITICAL_CHANCE 10						// �N���e�B�J���m��
#define PUNISHER_DEFAULT_BAG_NUM 0						// �S�~�܂̐��̏����l
#define PUNISHER_DEFAULT_GOLD_BAG_NUM 10				// �S�[���h�S�~�܂̐��̏����l

// �R���X�g���N�^
CTrashEnemyStatus::CTrashEnemyStatus(bool punisher)
	: CTrashStatusBase(NORMAL_MAX_HP, NORMAL_BASE_MOVE_SPEED, NORMAL_JUMP_SPEED,
		NORMAL_KNOCKBACK_SPEED, NORMAL_ATTACK_POWER, NORMAL_POWER)
	, mCriticalChance(NORMAL_CRITICAL_CHANCE)
	, mIsPunisher(punisher)
	, mDefaultBagNum(0)
	, mDefaultGoldBagNum(0)
{
	// ���d�u���p�Ȃ�
	if (punisher)
	{
		// ���d�u���p�̃X�e�[�^�X�ɕύX����
		SetAllStatus(PUNISHER_MAX_HP, PUNISHER_BASE_MOVE_SPEED, PUNISHER_JUMP_SPEED,
			PUNISHER_KNOCKBACK_SPEED, PUNISHER_ATTACK_POWER, PUNISHER_POWER);
		mCriticalChance = PUNISHER_CRITICAL_CHANCE;
		mDefaultBagNum = PUNISHER_DEFAULT_BAG_NUM;
		mDefaultGoldBagNum = PUNISHER_DEFAULT_GOLD_BAG_NUM;
	}
}

// �f�X�g���N�^
CTrashEnemyStatus::~CTrashEnemyStatus()
{
}

// �N���e�B�J���m�����擾
int CTrashEnemyStatus::GetCriticalChance()
{
	return mCriticalChance;
}

// �N���e�B�J���m����ݒ�
void CTrashEnemyStatus::SetCriticalChance(int criticalChance)
{
	mCriticalChance = criticalChance;
}

// �S�~�܂̐��̏����l���擾����
int CTrashEnemyStatus::GetDefaultBagNum() const
{
	return mDefaultBagNum;
}

// �S�[���h�S�~�܂̐��̏����l���擾����
int CTrashEnemyStatus::GetDefaultGoldBagNum() const
{
	return mDefaultGoldBagNum;
}

// �S�~�܂̐������Z�b�g
void CTrashEnemyStatus::ResetBag()
{
	// �S�~�܂̐����擾
	int bagNum = GetTrashBag();
	// �S�~�܂̐����[���ɂ���
	SetTrashBag(-bagNum);
	// �����l��ݒ�
	SetTrashBag(GetDefaultBagNum());

	// �S�[���h�S�~�܂̐����擾
	bagNum = GetGoldTrashBag();
	// �S�[���h�S�~�܂̐����[���ɂ���
	SetGoldTrashBag(-bagNum);
	// �����l��ݒ�
	SetGoldTrashBag(GetDefaultGoldBagNum());
}
