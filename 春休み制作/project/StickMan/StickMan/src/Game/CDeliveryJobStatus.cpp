#include "CDeliveryJobStatus.h"

// アンロック金額
#define UNLOCK_MONEY 2000

CDeliveryJobStatus::CDeliveryJobStatus()
	: CJobStatusBase(EJobType::eDelivery)
{
	// アンロック金額を設定
	SetUnlockMoney(UNLOCK_MONEY);
}

CDeliveryJobStatus::~CDeliveryJobStatus()
{
}
