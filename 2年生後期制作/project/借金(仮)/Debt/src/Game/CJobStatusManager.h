#pragma once
#include "CJobStatusBase.h"
class CTrashJobStatus;
class CDeliveryStatus;

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

	CJobStatusManager();
	~CJobStatusManager();

	void Update();

	// TODO
	// �e�d���̃A�����b�N��Ԃ��擾����
	bool GetUnlock(EJobType jobType) const;
	/// <summary>
	/// �e�d���̃A�����b�N��Ԃ�ݒ肷��
	/// </summary>
	/// <param name="jobType">�d���̎��</param>
	/// <param name="unlock">�A�����b�N���(true�ŃA�����b�N)</param>
	void SetUnlock(EJobType jobType, bool unlock);

	// �e�d���̋����{�����擾����
	float GetUpgrade(EJobType jobType) const;
	/// <summary>
	/// �e�d���̋����{����ݒ肷��
	/// </summary>
	/// <param name="jobType">�d���̎��</param>
	/// <param name="upgrade">�����{���̑�����</param>
	void SetUpgrade(EJobType jobType, float upgrade);

	// �e�W���u�̃|�C���^���擾
	CTrashJobStatus* GetTrashStatus();
	CDeliveryStatus* GetDeliveryStatus();

	// �I������Ă���d�����擾
	EJobType GetSelectJob() const;
	// �I������Ă���d����ݒ�
	void SetSelectJob(EJobType jobType);
private:
	// �I������Ă���d��
	EJobType mSelectJob;
	// �d���X�e�[�^�X�}�l�[�W���̃C���X�^���X
	static CJobStatusManager* mpInstance;
	// �e�W���u�̃X�e�[�^�X
	CTrashJobStatus* mTrashStatus;
	CDeliveryStatus* mDeliveryStatus;
};