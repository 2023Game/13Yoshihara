#include "CJobStatusManager.h"
#include "CTrashJobStatus.h"
#include "CDeliveryJobStatus.h"

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

// コンストラクタ
CJobStatusManager::CJobStatusManager()
	: mSelectJob(EJobType::eNone)
{
	mTrashStatus = new CTrashJobStatus();
	mDeliveryStatus = new CDeliveryJobStatus();
}

// デストラクタ
CJobStatusManager::~CJobStatusManager()
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

// 各仕事のアンロック金額を取得
int CJobStatusManager::GetUnlockMoney(EJobType jobType) const
{
	switch (jobType)
	{
		// ゴミ拾いの場合
	case EJobType::eTrash:

		return mTrashStatus->GetUnlockMoney();

		break;

		// 配達の場合
	case EJobType::eDelivery:

		return mDeliveryStatus->GetUnlockMoney();

		break;
		// それ以外
	default:

		return false;
	}
}

// 各仕事のアンロック金額を設定
void CJobStatusManager::SetUnlockMoney(EJobType jobType, int unlockMoney)
{
	switch (jobType)
	{
		// ゴミ拾いの場合
	case EJobType::eTrash:

		mTrashStatus->SetUnlockMoney(unlockMoney);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mDeliveryStatus->SetUnlockMoney(unlockMoney);

		break;

	default:

		return;

		break;
	}
}

// 各仕事のボーナスの強化値を取得
float CJobStatusManager::GetBonus(EJobType jobType) const
{
	switch (jobType)
	{
	// ゴミ拾いの場合
	case EJobType::eTrash:

		return mTrashStatus->GetBonus();

		break;
	// 配達の場合
	case EJobType::eDelivery:

		return mDeliveryStatus->GetBonus();

		break;

	default:

		return 1.0f;

		break;
	}
}

// 各仕事のボーナスの強化値を設定
void CJobStatusManager::SetBonus(EJobType jobType, float bonus)
{
	switch (jobType)
	{
	// ゴミ拾いの場合
	case EJobType::eTrash:

		mTrashStatus->SetBonus(bonus);

		break;

	// 配達の場合
	case EJobType::eDelivery:

		mDeliveryStatus->SetBonus(bonus);

		break;

	default:

		return;

		break;
	}
}

// 各仕事のHPの強化値を取得
int CJobStatusManager::GetHp(EJobType jobType) const
{
	switch (jobType)
	{
		// ゴミ拾いの場合
	case EJobType::eTrash:

		return mTrashStatus->GetHp();

		break;

		// 配達の場合
	case EJobType::eDelivery:

		return mDeliveryStatus->GetHp();

		break;
		// それ以外
	default:

		return false;
	}
}

// 各仕事のHPの強化値を設定
void CJobStatusManager::SetHp(EJobType jobType, int hp)
{
	switch (jobType)
	{
		// ゴミ拾いの場合
	case EJobType::eTrash:

		mTrashStatus->SetHp(hp);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mDeliveryStatus->SetHp(hp);

		break;

	default:

		return;

		break;
	}
}

// 各仕事の移動速度の強化値を取得
float CJobStatusManager::GetSpeed(EJobType jobType) const
{
	switch (jobType)
	{
		// ゴミ拾いの場合
	case EJobType::eTrash:

		return mTrashStatus->GetSpeed();

		break;

		// 配達の場合
	case EJobType::eDelivery:

		return mDeliveryStatus->GetSpeed();

		break;
		// それ以外
	default:

		return false;
	}
}

// 各仕事の移動速度の強化値を設定
void CJobStatusManager::SetSpeed(EJobType jobType, float speed)
{
	switch (jobType)
	{
		// ゴミ拾いの場合
	case EJobType::eTrash:

		mTrashStatus->SetSpeed(speed);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mDeliveryStatus->SetSpeed(speed);

		break;

	default:

		return;

		break;
	}
}

// 各仕事の攻撃力の強化値を取得
int CJobStatusManager::GetPower(EJobType jobType) const
{
	switch (jobType)
	{
		// ゴミ拾いの場合
	case EJobType::eTrash:

		return mTrashStatus->GetPower();

		break;

		// 配達の場合
	case EJobType::eDelivery:

		return mDeliveryStatus->GetPower();

		break;
		// それ以外
	default:

		return false;
	}
}

// 各仕事の攻撃力の強化値を設定
void CJobStatusManager::SetPower(EJobType jobType, int power)
{
	switch (jobType)
	{
		// ゴミ拾いの場合
	case EJobType::eTrash:

		mTrashStatus->SetPower(power);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mDeliveryStatus->SetPower(power);

		break;

	default:

		return;

		break;
	}
}

//// ゴミ拾いのステータスのポインタを取得
//CTrashJobStatus* CJobStatusManager::GetTrashStatus()
//{
//	return mTrashStatus;
//}
//
//// 配達のステータスのポインタを取得
//CDeliveryJobStatus* CJobStatusManager::GetDeliveryStatus()
//{
//	return mDeliveryStatus;
//}

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
