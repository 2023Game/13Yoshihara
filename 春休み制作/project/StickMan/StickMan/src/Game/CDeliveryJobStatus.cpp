#include "CDeliveryJobStatus.h"

// �A�����b�N���z
#define UNLOCK_MONEY 2000

CDeliveryJobStatus::CDeliveryJobStatus()
	: CJobStatusBase(EJobType::eDelivery)
{
	// �A�����b�N���z��ݒ�
	SetUnlockMoney(UNLOCK_MONEY);
}

CDeliveryJobStatus::~CDeliveryJobStatus()
{
}
