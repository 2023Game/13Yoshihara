#include "CTrashJobStatus.h"

// アンロック金額
#define UNLOCK_MONEY 2000

CTrashJobStatus::CTrashJobStatus()
	: CJobStatusBase(EJobType::eTrash)
{
	// アンロック金額を設定
	SetUnlockMoney(UNLOCK_MONEY);
}

CTrashJobStatus::~CTrashJobStatus()
{
}
