#include "CJobStatusBase.h"

// �R���X�g���N�^
CJobStatusBase::CJobStatusBase(EJobType jobType)
	: mJobType(jobType)
	, mUnlock(false)
	, mBonusUpgrade(0.0f)
	, mHpUpgrade(0)
	, mSpeedUpgrade(0.0f)
	, mPowerUpgrade(0)

{
}

// �f�X�g���N�^
CJobStatusBase::~CJobStatusBase()
{
}

// �X�V
void CJobStatusBase::Update()
{
}

// �A�����b�N��Ԃ��擾
bool CJobStatusBase::GetUnlock() const
{
	return mUnlock;
}

// �A�����b�N��Ԃ�ݒ�
void CJobStatusBase::SetUnlock(bool unlock)
{
	mUnlock = unlock;
}

// �A�����b�N�̋��z���擾
int CJobStatusBase::GetUnlockMoney() const
{
	return mUnlockMoney;
}

// �A�����b�N�̋��z��ݒ�
void CJobStatusBase::SetUnlockMoney(int unlockMoney)
{
	mUnlockMoney = unlockMoney;
}

// �{�[�i�X�̋����l���擾
float CJobStatusBase::GetBonus() const
{
	return mBonusUpgrade;
}

// �{�[�i�X�̋����l��ݒ�
void CJobStatusBase::SetBonus(float bonus)
{
	mBonusUpgrade = bonus;
}

// HP�̋����l���擾
int CJobStatusBase::GetHp() const
{
	return mHpUpgrade;
}

// HP�̋����l��ݒ�
void CJobStatusBase::SetHp(int hp)
{
	mHpUpgrade = hp;
}

// �ړ����x�̋����l���擾
float CJobStatusBase::GetSpeed() const
{
	return mSpeedUpgrade;
}

// �ړ����x�̋����l��ݒ�
void CJobStatusBase::SetSpeed(float speed)
{
	mSpeedUpgrade = speed;
}

// �U���͂̋����l���擾
int CJobStatusBase::GetPower() const
{
	return mPowerUpgrade;
}

// �U���͂̋����l��ݒ�
void CJobStatusBase::SetPower(int power)
{
	mPowerUpgrade = power;
}

// �d���̎�ނ̎擾
EJobType CJobStatusBase::GetJobType()
{
	return mJobType;
}
