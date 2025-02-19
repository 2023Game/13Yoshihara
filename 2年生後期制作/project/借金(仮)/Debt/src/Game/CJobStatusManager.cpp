#include "CJobStatusManager.h"
#include "CTrashJobStatus.h"
#include "CDeliveryJobStatus.h"

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

// �R���X�g���N�^
CJobStatusManager::CJobStatusManager()
	: mSelectJob(EJobType::eNone)
{
	mTrashStatus = new CTrashJobStatus();
	mDeliveryStatus = new CDeliveryJobStatus();
}

// �f�X�g���N�^
CJobStatusManager::~CJobStatusManager()
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

// �e�d���̃A�����b�N���z���擾
int CJobStatusManager::GetUnlockMoney(EJobType jobType) const
{
	switch (jobType)
	{
		// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		return mTrashStatus->GetUnlockMoney();

		break;

		// �z�B�̏ꍇ
	case EJobType::eDelivery:

		return mDeliveryStatus->GetUnlockMoney();

		break;
		// ����ȊO
	default:

		return false;
	}
}

// �e�d���̃A�����b�N���z��ݒ�
void CJobStatusManager::SetUnlockMoney(EJobType jobType, int unlockMoney)
{
	switch (jobType)
	{
		// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		mTrashStatus->SetUnlockMoney(unlockMoney);

		break;

		// �z�B�̏ꍇ
	case EJobType::eDelivery:

		mDeliveryStatus->SetUnlockMoney(unlockMoney);

		break;

	default:

		return;

		break;
	}
}

// �e�d���̃{�[�i�X�̋����l���擾
float CJobStatusManager::GetBonus(EJobType jobType) const
{
	switch (jobType)
	{
	// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		return mTrashStatus->GetBonus();

		break;
	// �z�B�̏ꍇ
	case EJobType::eDelivery:

		return mDeliveryStatus->GetBonus();

		break;

	default:

		return 1.0f;

		break;
	}
}

// �e�d���̃{�[�i�X�̋����l��ݒ�
void CJobStatusManager::SetBonus(EJobType jobType, float bonus)
{
	switch (jobType)
	{
	// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		mTrashStatus->SetBonus(bonus);

		break;

	// �z�B�̏ꍇ
	case EJobType::eDelivery:

		mDeliveryStatus->SetBonus(bonus);

		break;

	default:

		return;

		break;
	}
}

// �e�d����HP�̋����l���擾
int CJobStatusManager::GetHp(EJobType jobType) const
{
	switch (jobType)
	{
		// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		return mTrashStatus->GetHp();

		break;

		// �z�B�̏ꍇ
	case EJobType::eDelivery:

		return mDeliveryStatus->GetHp();

		break;
		// ����ȊO
	default:

		return false;
	}
}

// �e�d����HP�̋����l��ݒ�
void CJobStatusManager::SetHp(EJobType jobType, int hp)
{
	switch (jobType)
	{
		// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		mTrashStatus->SetHp(hp);

		break;

		// �z�B�̏ꍇ
	case EJobType::eDelivery:

		mDeliveryStatus->SetHp(hp);

		break;

	default:

		return;

		break;
	}
}

// �e�d���̈ړ����x�̋����l���擾
float CJobStatusManager::GetSpeed(EJobType jobType) const
{
	switch (jobType)
	{
		// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		return mTrashStatus->GetSpeed();

		break;

		// �z�B�̏ꍇ
	case EJobType::eDelivery:

		return mDeliveryStatus->GetSpeed();

		break;
		// ����ȊO
	default:

		return false;
	}
}

// �e�d���̈ړ����x�̋����l��ݒ�
void CJobStatusManager::SetSpeed(EJobType jobType, float speed)
{
	switch (jobType)
	{
		// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		mTrashStatus->SetSpeed(speed);

		break;

		// �z�B�̏ꍇ
	case EJobType::eDelivery:

		mDeliveryStatus->SetSpeed(speed);

		break;

	default:

		return;

		break;
	}
}

// �e�d���̍U���͂̋����l���擾
int CJobStatusManager::GetPower(EJobType jobType) const
{
	switch (jobType)
	{
		// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		return mTrashStatus->GetPower();

		break;

		// �z�B�̏ꍇ
	case EJobType::eDelivery:

		return mDeliveryStatus->GetPower();

		break;
		// ����ȊO
	default:

		return false;
	}
}

// �e�d���̍U���͂̋����l��ݒ�
void CJobStatusManager::SetPower(EJobType jobType, int power)
{
	switch (jobType)
	{
		// �S�~�E���̏ꍇ
	case EJobType::eTrash:

		mTrashStatus->SetPower(power);

		break;

		// �z�B�̏ꍇ
	case EJobType::eDelivery:

		mDeliveryStatus->SetPower(power);

		break;

	default:

		return;

		break;
	}
}

//// �S�~�E���̃X�e�[�^�X�̃|�C���^���擾
//CTrashJobStatus* CJobStatusManager::GetTrashStatus()
//{
//	return mTrashStatus;
//}
//
//// �z�B�̃X�e�[�^�X�̃|�C���^���擾
//CDeliveryJobStatus* CJobStatusManager::GetDeliveryStatus()
//{
//	return mDeliveryStatus;
//}

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
