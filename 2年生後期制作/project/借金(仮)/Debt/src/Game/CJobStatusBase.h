#pragma once
#include "JobType.h"

// �d���̃X�e�[�^�X�̊��N���X
class CJobStatusBase
{
public:
	// �x�[�X�̎d���̃X�e�[�^�X
	struct BaseJobStatus
	{
		// �A�����b�N��ԁi�����lfalse�Ŗ������j
		bool mUnlock;
		// �A�����b�N���z
		int mUnlockMoney;

		// �{�[�i�X�̋����l�i�����l0.0f�j
		float mBonusUpgrade;
		// HP�̋����l�i�����l0�j
		int mHpUpgrade;
		// �ړ����x�̋����l�i�����l0.0f�j
		float mSpeedUpgrade;
		// �U���͂̋����l�i�����l0�j
		int mPowerUpgrade;
		BaseJobStatus() : mUnlock(false), mUnlockMoney(0.0f),
			mBonusUpgrade(0.0f), mHpUpgrade(0), mSpeedUpgrade(0.0f),
			mPowerUpgrade(0) {}
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="jobType">�d���̎��</param>
	CJobStatusBase(EJobType jobType);
	// �f�X�g���N�^
	~CJobStatusBase();

	// �A�����b�N��Ԃ��擾
	bool GetUnlock() const;
	// �A�����b�N��Ԃ�ݒ�
	void SetUnlock(bool unlock);

	// �A�����b�N�̋��z���擾
	int GetUnlockMoney() const;
	// �A�����b�N�̋��z��ݒ�
	void SetUnlockMoney(int unlockMoney);

	// �{�[�i�X�̋����l���擾
	float GetBonus() const;
	// �{�[�i�X�̋����l��ݒ�
	void SetBonus(float bonus);

	// HP�̋����l���擾
	int GetHp() const;
	// HP�̋����l��ݒ�
	void SetHp(int hp);

	// �ړ����x�̋����l���擾
	float GetSpeed() const;
	// �ړ����x�̋����l��ݒ�
	void SetSpeed(float speed);

	// �U���͂̋����l���擾
	int GetPower() const;
	// �U���͂̋����l��ݒ�
	void SetPower(int power);

	// �O�̓��̎d���̃X�e�[�^�X�����̃X�e�[�^�X�ɐݒ�
	void SetPreBaseJobStatus();
	// �X�e�[�^�X��O���Ƀ��[���o�b�N����
	void Rollback();

private:
	// �x�[�X�̎d���̃X�e�[�^�X
	BaseJobStatus mBaseJobStatus;
	// �O�̓��̎d���̃X�e�[�^�X
	BaseJobStatus mPreBaseJobStatus;
};