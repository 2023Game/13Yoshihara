#include "CJobStatusBase.h"

// �R���X�g���N�^
CJobStatusBase::CJobStatusBase(EJobType jobType)
	: mIsFirstPlay(true)
{
}

// �f�X�g���N�^
CJobStatusBase::~CJobStatusBase()
{
}

// �A�����b�N��Ԃ��擾
bool CJobStatusBase::GetUnlock() const
{
	return mBaseJobStatus.mUnlock;
}

// �A�����b�N��Ԃ�ݒ�
void CJobStatusBase::SetUnlock(bool unlock)
{
	mBaseJobStatus.mUnlock = unlock;
}

// �A�����b�N�̋��z���擾
int CJobStatusBase::GetUnlockMoney() const
{
	return mBaseJobStatus.mUnlockMoney;
}

// �A�����b�N�̋��z��ݒ�
void CJobStatusBase::SetUnlockMoney(int unlockMoney)
{
	mBaseJobStatus.mUnlockMoney = unlockMoney;
}

// �{�[�i�X�̋����l���擾
float CJobStatusBase::GetBonus() const
{
	return mBaseJobStatus.mBonusUpgrade;
}

// �{�[�i�X�̋����l��ݒ�
void CJobStatusBase::SetBonus(float bonus)
{
	mBaseJobStatus.mBonusUpgrade = bonus;
}

// HP�̋����l���擾
int CJobStatusBase::GetHp() const
{
	return mBaseJobStatus.mHpUpgrade;
}

// HP�̋����l��ݒ�
void CJobStatusBase::SetHp(int hp)
{
	mBaseJobStatus.mHpUpgrade = hp;
}

// �ړ����x�̋����l���擾
float CJobStatusBase::GetSpeed() const
{
	return mBaseJobStatus.mSpeedUpgrade;
}

// �ړ����x�̋����l��ݒ�
void CJobStatusBase::SetSpeed(float speed)
{
	mBaseJobStatus.mSpeedUpgrade = speed;
}

// �U���͂̋����l���擾
int CJobStatusBase::GetPower() const
{
	return mBaseJobStatus.mPowerUpgrade;
}

// �U���͂̋����l��ݒ�
void CJobStatusBase::SetPower(int power)
{
	mBaseJobStatus.mPowerUpgrade = power;
}

// �O�̓��̎d���̃X�e�[�^�X�����̃X�e�[�^�X�ɐݒ�
void CJobStatusBase::SetPreBaseJobStatus()
{
	mPreBaseJobStatus = mBaseJobStatus;
}

// �X�e�[�^�X��O���Ƀ��[���o�b�N����
void CJobStatusBase::Rollback()
{
	mBaseJobStatus = mPreBaseJobStatus;
}

// ���߂Ẵv���C�����擾
bool CJobStatusBase::GetFirstPlay() const
{
	return mIsFirstPlay;
}

// ���߂Ẵv���C����ݒ�
void CJobStatusBase::SetFirstPlay(bool isFirst)
{
	mIsFirstPlay = isFirst;
}
