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
	, mPreSelectJob(EJobType::eNone)
{
	mpTrashStatus = new CTrashJobStatus();
	mpDeliveryStatus = new CDeliveryJobStatus();
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
		
		return mpTrashStatus->GetUnlock();
		
		break;

	// 配達の場合
	case EJobType::eDelivery:

		return mpDeliveryStatus->GetUnlock();
		
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

		mpTrashStatus->SetUnlock(unlock);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mpDeliveryStatus->SetUnlock(unlock);

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

		return mpTrashStatus->GetUnlockMoney();

		break;

		// 配達の場合
	case EJobType::eDelivery:

		return mpDeliveryStatus->GetUnlockMoney();

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

		mpTrashStatus->SetUnlockMoney(unlockMoney);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mpDeliveryStatus->SetUnlockMoney(unlockMoney);

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

		return mpTrashStatus->GetBonus();

		break;
	// 配達の場合
	case EJobType::eDelivery:

		return mpDeliveryStatus->GetBonus();

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

		mpTrashStatus->SetBonus(bonus);

		break;

	// 配達の場合
	case EJobType::eDelivery:

		mpDeliveryStatus->SetBonus(bonus);

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

		return mpTrashStatus->GetHp();

		break;

		// 配達の場合
	case EJobType::eDelivery:

		return mpDeliveryStatus->GetHp();

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

		mpTrashStatus->SetHp(hp);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mpDeliveryStatus->SetHp(hp);

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

		return mpTrashStatus->GetSpeed();

		break;

		// 配達の場合
	case EJobType::eDelivery:

		return mpDeliveryStatus->GetSpeed();

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

		mpTrashStatus->SetSpeed(speed);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mpDeliveryStatus->SetSpeed(speed);

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

		return mpTrashStatus->GetPower();

		break;

		// 配達の場合
	case EJobType::eDelivery:

		return mpDeliveryStatus->GetPower();

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

		mpTrashStatus->SetPower(power);

		break;

		// 配達の場合
	case EJobType::eDelivery:

		mpDeliveryStatus->SetPower(power);

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

// 全ての前の日のステータスを今のステータスに設定
void CJobStatusManager::SetPreJobStatus()
{
	// 全ての前の日のステータスを今のステータスに設定
	mpTrashStatus->SetPreBaseJobStatus();
	mpDeliveryStatus->SetPreBaseJobStatus();
	// 前の日に選択されていた仕事を設定
	mPreSelectJob = mSelectJob;
}

// 前日にロールバックする
void CJobStatusManager::Rollback()
{
	// 全てのステータスをロールバックする
	mpTrashStatus->Rollback();
	mpDeliveryStatus->Rollback();
	// 選択されている仕事を前日のものに設定
	mSelectJob = mPreSelectJob;
}
