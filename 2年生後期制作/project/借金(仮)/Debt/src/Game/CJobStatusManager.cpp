#include "CJobStatusManager.h"
#include "CTrashStatus.h"
#include "CDeliveryStatus.h"

// 仕事ステータスマネージャのインスタンス
CJobStatusManager* CJobStatusManager::mpInstance = nullptr;

// インスタンスを取得
CJobStatusManager* CJobStatusManager::Instance()
{
	// インスタンスが無ければ、新しく生成する
	if (mpInstance == nullptr)
	{
		mpInstance = new CJobStatusManager();
	}
	return mpInstance;
}

// インスタンスの破棄
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
