#pragma once
#include "CJobStatusBase.h"
class CTrashJobStatus;
class CDeliveryJobStatus;

class CJobStatusManager
{
public:
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static CJobStatusManager* Instance();
	// �C���X�^���X��j��
	void ClearInstance();

	// �R���X�g���N�^
	CJobStatusManager();
	// �f�X�g���N�^
	~CJobStatusManager();

	// �e�d���̃A�����b�N��Ԃ��擾����
	bool GetUnlock(EJobType jobType) const;
	// �e�d���̃A�����b�N��Ԃ�ݒ肷��
	void SetUnlock(EJobType jobType, bool unlock);

	// �e�d���̃A�����b�N���z���擾
	int GetUnlockMoney(EJobType jobType) const;
	// �e�d���̃A�����b�N���z��ݒ�
	void SetUnlockMoney(EJobType jobType, int unlockMoney);

	// �e�d���̃{�[�i�X�̋����l���擾����
	float GetBonus(EJobType jobType) const;
	// �e�d���̃{�[�i�X�̋����l��ݒ肷��
	void SetBonus(EJobType jobType, float bonus);

	// �e�d����HP�̋����l���擾
	int GetHp(EJobType jobType) const;
	// �e�d����HP�̋����l��ݒ�
	void SetHp(EJobType jobType, int hp);

	// �e�d���̈ړ����x�̋����l���擾
	float GetSpeed(EJobType jobType) const;
	// �e�d���̈ړ����x�̋����l��ݒ�
	void SetSpeed(EJobType jobType, float speed);

	// �e�d���̍U���͂̋����l���擾
	int GetPower(EJobType jobType) const;
	// �e�d���̍U���͂̋����l��ݒ�
	void SetPower(EJobType jobType, int power);

	//// �e�W���u�̃|�C���^���擾
	//CTrashJobStatus* GetTrashStatus();
	//CDeliveryJobStatus* GetDeliveryStatus();

	// �I������Ă���d�����擾
	EJobType GetSelectJob() const;
	// �I������Ă���d����ݒ�
	void SetSelectJob(EJobType jobType);

	// �S�Ă̑O�̓��̃X�e�[�^�X�����̃X�e�[�^�X�ɐݒ�
	void SetPreJobStatus();
	// �O���Ƀ��[���o�b�N����
	void Rollback();

	// ���߂Ẵv���C�����擾
	bool GetFirstPlay(EJobType jobType);
	// ���߂Ẵv���C���ݒ�
	void SetFirstPlay(EJobType jobType, bool isFirst);

private:
	// �I������Ă���d��
	EJobType mSelectJob;
	// �O�̓��ɑI������Ă����d��
	EJobType mPreSelectJob;
	// �d���X�e�[�^�X�}�l�[�W���̃C���X�^���X
	static CJobStatusManager* mpInstance;
	// �e�W���u�̃X�e�[�^�X
	CTrashJobStatus* mpTrashStatus;
	CDeliveryJobStatus* mpDeliveryStatus;
};