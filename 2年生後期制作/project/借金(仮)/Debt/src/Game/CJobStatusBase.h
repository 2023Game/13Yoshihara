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
	~CJobStatusBase();

	void Update();

	// �A�����b�N��Ԃ��擾
	bool GetUnlock();
	// �A�����b�N��Ԃ�ݒ�
	void SetUnlock(bool unlock);

	// �����{�����擾
	float GetUpgrade();
	/// <summary>
	/// �����{����ݒ�
	/// </summary>
	/// <param name="upgrade">�����{���̑�����</param>
	void SetUpgrade(float upgrade);

	// �d���̎�ނ��擾
	EJobType GetJobType();

private:
	// �d���X�e�[�^�X�}�l�[�W���̃C���X�^���X
	CJobStatusBase* mpInstance;

	// �d���̎��
	EJobType mJobType;
	
	// �A�����b�N��ԁi�����lfalse�Ŗ������j
	bool mUnlock;
	
	// �����{���i�����l1.0f�j
	float mUpgrade;
};