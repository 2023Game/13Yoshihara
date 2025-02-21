#include "CJobStatusBase.h"

// コンストラクタ
CJobStatusBase::CJobStatusBase(EJobType jobType)
	: mIsFirstPlay(true)
{
}

// デストラクタ
CJobStatusBase::~CJobStatusBase()
{
}

// アンロック状態を取得
bool CJobStatusBase::GetUnlock() const
{
	return mBaseJobStatus.mUnlock;
}

// アンロック状態を設定
void CJobStatusBase::SetUnlock(bool unlock)
{
	mBaseJobStatus.mUnlock = unlock;
}

// アンロックの金額を取得
int CJobStatusBase::GetUnlockMoney() const
{
	return mBaseJobStatus.mUnlockMoney;
}

// アンロックの金額を設定
void CJobStatusBase::SetUnlockMoney(int unlockMoney)
{
	mBaseJobStatus.mUnlockMoney = unlockMoney;
}

// ボーナスの強化値を取得
float CJobStatusBase::GetBonus() const
{
	return mBaseJobStatus.mBonusUpgrade;
}

// ボーナスの強化値を設定
void CJobStatusBase::SetBonus(float bonus)
{
	mBaseJobStatus.mBonusUpgrade = bonus;
}

// HPの強化値を取得
int CJobStatusBase::GetHp() const
{
	return mBaseJobStatus.mHpUpgrade;
}

// HPの強化値を設定
void CJobStatusBase::SetHp(int hp)
{
	mBaseJobStatus.mHpUpgrade = hp;
}

// 移動速度の強化値を取得
float CJobStatusBase::GetSpeed() const
{
	return mBaseJobStatus.mSpeedUpgrade;
}

// 移動速度の強化値を設定
void CJobStatusBase::SetSpeed(float speed)
{
	mBaseJobStatus.mSpeedUpgrade = speed;
}

// 攻撃力の強化値を取得
int CJobStatusBase::GetPower() const
{
	return mBaseJobStatus.mPowerUpgrade;
}

// 攻撃力の強化値を設定
void CJobStatusBase::SetPower(int power)
{
	mBaseJobStatus.mPowerUpgrade = power;
}

// 前の日の仕事のステータスを今のステータスに設定
void CJobStatusBase::SetPreBaseJobStatus()
{
	mPreBaseJobStatus = mBaseJobStatus;
}

// ステータスを前日にロールバックする
void CJobStatusBase::Rollback()
{
	mBaseJobStatus = mPreBaseJobStatus;
}

// 初めてのプレイかを取得
bool CJobStatusBase::GetFirstPlay() const
{
	return mIsFirstPlay;
}

// 初めてのプレイかを設定
void CJobStatusBase::SetFirstPlay(bool isFirst)
{
	mIsFirstPlay = isFirst;
}
