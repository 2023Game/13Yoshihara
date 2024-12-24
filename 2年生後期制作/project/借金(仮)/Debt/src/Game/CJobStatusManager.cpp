#include "CJobStatusManager.h"
#include "CTrashJobStatus.h"
#include "CDeliveryStatus.h"

// �d���X�e�[�^�X�}�l�[�W���̃C���X�^���X
CJobStatusManager* CJobStatusManager::mpInstance = nullptr;

// �C���X�^���X���擾
CJobStatusManager* CJobStatusManager::Instance()
{
	// �C���X�^���X��������΁A�V������������
	if (mpInstance == nullptr)
	{
		mpInstance = new CJobStatusManager();
	}
	return mpInstance;
}

// �C���X�^���X�̔j��
void CJobStatusManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

CJobStatusManager::CJobStatusManager()
	: mSelectJob(EJobType::eNone)
{
	mTrashStatus = new CTrashJobStatus();
	mDeliveryStatus = new CDeliveryStatus();
}

CJobStatusManager::~CJobStatusManager()
{
}

void CJobStatusManager::Update()
{
}

// �e�d���̃A�����b�N��Ԃ��擾
bool CJobStatusManager::GetUnlock(EJobType jobType) const
{
	switch (jobType)
	{
	// �S�~�E���̏ꍇ
	case EJobType::eTrash:
		
		return mTrashStatus->GetUnlock();
		
		break;

	// �z�B�̏ꍇ
	case EJobType::eDelivery:

		return mDeliveryStatus->GetUnlock();
		
		break;
	// ����ȊO
	default:
		
		return false;
	}
}

// �e�d���̃A�����b�N��Ԃ�ݒ�
void CJobStatusManager::SetUnlock(EJobType jobType, bool unlock)
{
	switch (jobType)
	{
		// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		mTrashStatus->SetUnlock(unlock);

		break;

		// �z�B�̏ꍇ
	case EJobType::eDelivery:

		mDeliveryStatus->SetUnlock(unlock);

		break;

	default:

		return;

		break;
	}
}

// �e�d���̋����l���擾
float CJobStatusManager::GetUpgrade(EJobType jobType) const
{
	switch (jobType)
	{
	// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		return mTrashStatus->GetUpgrade();

		break;
	// �z�B�̏ꍇ
	case EJobType::eDelivery:

		return mDeliveryStatus->GetUpgrade();

		break;

	default:

		return 1.0f;

		break;
	}
}

// �e�d���̋����l��ݒ�
void CJobStatusManager::SetUpgrade(EJobType jobType, float upgrade)
{
	switch (jobType)
	{
	// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		mTrashStatus->SetUpgrade(upgrade);

		break;

	// �z�B�̏ꍇ
	case EJobType::eDelivery:

		mDeliveryStatus->SetUpgrade(upgrade);

		break;

	default:

		return;

		break;
	}
}

// �S�~�E���̃X�e�[�^�X�̃|�C���^���擾
CTrashJobStatus* CJobStatusManager::GetTrashStatus()
{
	return mTrashStatus;
}

// �z�B�̃X�e�[�^�X�̃|�C���^���擾
CDeliveryStatus* CJobStatusManager::GetDeliveryStatus()
{
	return mDeliveryStatus;
}

// �I������Ă���d�����擾
EJobType CJobStatusManager::GetSelectJob() const
{
	return mSelectJob;
}

// �I������Ă���d����ݒ�
void CJobStatusManager::SetSelectJob(EJobType jobType)
{
	mSelectJob = jobType;
}
