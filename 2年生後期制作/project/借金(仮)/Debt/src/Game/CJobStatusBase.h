#pragma once
#include "JobType.h"

class CJobStatusBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="jobType">�d���̎��</param>
	CJobStatusBase(EJobType jobType);
	// �f�X�g���N�^
	~CJobStatusBase();

	// �X�V
	void Update();

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

	// �d���̎�ނ��擾
	EJobType GetJobType();

private:
	// �d���̎��
	EJobType mJobType;
	
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
};