#include "CJobStatusManager.h"
#include "CTrashStatus.h"
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
{
	mTrashStatus = new CTrashStatus();
	mDeliveryStatus = new CDeliveryStatus();
}

CJobStatusManager::~CJobStatusManager()
{
}

void CJobStatusManager::Update()
{
}
