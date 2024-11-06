#pragma once
#include "CJobStatusBase.h"
class CTrashStatus;
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
	bool GetUnlock(EJobType jobType);
	/// <summary>
	/// �e�d���̃A�����b�N��Ԃ�ݒ肷��
	/// </summary>
	/// <param name="jobType">�d���̎��</param>
	/// <param name="unlock">�A�����b�N���(true�ŃA�����b�N)</param>
	void SetUnlock(EJobType jobType, bool unlock);

	// �e�d���̋����{�����擾����
	float GetUpgrade(EJobType jobType);
	/// <summary>
	/// �e�d���̋����{����ݒ肷��
	/// </summary>
	/// <param name="jobType">�d���̎��</param>
	/// <param name="upgrade">�����{���̑�����</param>
	void SetUpgrade(EJobType jobType, float upgrade);
private:
	// �d���X�e�[�^�X�}�l�[�W���̃C���X�^���X
	static CJobStatusManager* mpInstance;
	CTrashStatus* mTrashStatus;
	CDeliveryStatus* mDeliveryStatus;
};