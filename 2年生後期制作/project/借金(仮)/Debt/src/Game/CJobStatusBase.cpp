#include "CJobStatusBase.h"

// コンストラクタ
CJobStatusBase::CJobStatusBase(EJobType jobType)
	: mJobType(jobType)
	, mUnlock(false)
	, mBonusUpgrade(0.0f)
	, mHpUpgrade(0)
	, mSpeedUpgrade(0.0f)
	, mPowerUpgrade(0)

{
}

// デストラクタ
CJobStatusBase::~CJobStatusBase()
{
}

// 更新
void CJobStatusBase::Update()
{
}

// アンロック状態を取得
bool CJobStatusBase::GetUnlock() const
{
	return mUnlock;
}

// アンロック状態を設定
void CJobStatusBase::SetUnlock(bool unlock)
{
	mUnlock = unlock;
}

// アンロックの金額を取得
int CJobStatusBase::GetUnlockMoney() const
{
	return mUnlockMoney;
}

// アンロックの金額を設定
void CJobStatusBase::SetUnlockMoney(int unlockMoney)
{
	mUnlockMoney = unlockMoney;
}

// ボーナスの強化値を取得
float CJobStatusBase::GetBonus() const
{
	return mBonusUpgrade;
}

// ボーナスの強化値を設定
void CJobStatusBase::SetBonus(float bonus)
{
	mBonusUpgrade = bonus;
}

// HPの強化値を取得
int CJobStatusBase::GetHp() const
{
	return mHpUpgrade;
}

// HPの強化値を設定
void CJobStatusBase::SetHp(int hp)
{
	mHpUpgrade = hp;
}

// 移動速度の強化値を取得
float CJobStatusBase::GetSpeed() const
{
	return mSpeedUpgrade;
}

// 移動速度の強化値を設定
void CJobStatusBase::SetSpeed(float speed)
{
	mSpeedUpgrade = speed;
}

// 攻撃力の強化値を取得
int CJobStatusBase::GetPower() const
{
	return mPowerUpgrade;
}

// 攻撃力の強化値を設定
void CJobStatusBase::SetPower(int power)
{
	mPowerUpgrade = power;
}

// 仕事の種類の取得
EJobType CJobStatusBase::GetJobType()
{
	return mJobType;
}
