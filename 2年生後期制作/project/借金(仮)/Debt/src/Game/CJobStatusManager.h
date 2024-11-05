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
	// �e�d���̉�����Ԃ��擾����

	// �e�d���̉�����Ԃ�ݒ肷��

	// �e�d���̋����l���擾����

	// �e�d���̋����l��ݒ肷��
private:
	// �d���X�e�[�^�X�}�l�[�W���̃C���X�^���X
	static CJobStatusManager* mpInstance;
	CTrashStatus* mTrashStatus;
	CDeliveryStatus* mDeliveryStatus;
};