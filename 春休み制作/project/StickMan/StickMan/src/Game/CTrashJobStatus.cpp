#include "CTrashJobStatus.h"

// �A�����b�N���z
#define UNLOCK_MONEY 2000

CTrashJobStatus::CTrashJobStatus()
	: CJobStatusBase(EJobType::eTrash)
{
	// �A�����b�N���z��ݒ�
	SetUnlockMoney(UNLOCK_MONEY);
}

CTrashJobStatus::~CTrashJobStatus()
{
}
