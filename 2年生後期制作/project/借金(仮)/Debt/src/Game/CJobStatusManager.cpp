#include "CJobStatusManager.h"
#include "CTrashJobStatus.h"
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

// 各仕事のアンロック状態を取得
bool CJobStatusManager::GetUnlock(EJobType jobType) const
{
	switch (jobType)
	{
	// ゴミ拾いの場合
	case EJobType::eTrash:
		
		return mTrashStatus->GetUnlock();
		
		break;

	// 配達の場合
	case EJobType::eDelivery:

		return mDeliveryStatus->GetUnlock();
		
		break;
	// それ以外
	default:
		
		return false;
	}
}

// 各仕事のアンロック状態を設定
void CJobStatusManager::SetUnlock(EJobType jobType, bool unlock)
{
	switch (jobType)
	{
		// ゴミ拾いの場合
	case EJobType::eTrash:

		mTrashStatus->SetUnlock(unlock);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mDeliveryStatus->SetUnlock(unlock);

		break;

	default:

		return;

		break;
	}
}

// 各仕事の強化値を取得
float CJobStatusManager::GetUpgrade(EJobType jobType) const
{
	switch (jobType)
	{
	// ゴミ拾いの場合
	case EJobType::eTrash:

		return mTrashStatus->GetUpgrade();

		break;
	// 配達の場合
	case EJobType::eDelivery:

		return mDeliveryStatus->GetUpgrade();

		break;

	default:

		return 1.0f;

		break;
	}
}

// 各仕事の強化値を設定
void CJobStatusManager::SetUpgrade(EJobType jobType, float upgrade)
{
	switch (jobType)
	{
	// ゴミ拾いの場合
	case EJobType::eTrash:

		mTrashStatus->SetUpgrade(upgrade);

		break;

	// 配達の場合
	case EJobType::eDelivery:

		mDeliveryStatus->SetUpgrade(upgrade);

		break;

	default:

		return;

		break;
	}
}

// ゴミ拾いのステータスのポインタを取得
CTrashJobStatus* CJobStatusManager::GetTrashStatus()
{
	return mTrashStatus;
}

// 配達のステータスのポインタを取得
CDeliveryStatus* CJobStatusManager::GetDeliveryStatus()
{
	return mDeliveryStatus;
}

// 選択されている仕事を取得
EJobType CJobStatusManager::GetSelectJob() const
{
	return mSelectJob;
}

// 選択されている仕事を設定
void CJobStatusManager::SetSelectJob(EJobType jobType)
{
	mSelectJob = jobType;
}
