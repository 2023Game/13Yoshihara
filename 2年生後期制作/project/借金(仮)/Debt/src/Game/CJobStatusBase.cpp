#include "CJobStatusBase.h"

CJobStatusBase::CJobStatusBase(EJobType jobType)
	: mJobType(jobType)
	, mUnlock(false)
	, mUpgrade(1.0f)
{
}

CJobStatusBase::~CJobStatusBase()
{
}

void CJobStatusBase::Update()
{
}

// アンロック状態を取得
bool CJobStatusBase::GetUnlock()
{
	return mUnlock;
}

// アンロック状態を設定
void CJobStatusBase::SetUnlock(bool unlock)
{
	mUnlock = unlock;
}

// 強化倍率を取得
float CJobStatusBase::GetUpgrade()
{
	return mUpgrade;
}

// 強化倍率を設定
void CJobStatusBase::SetUpgrade(float upgrade)
{
	mUpgrade = mUpgrade + upgrade;
}

// 仕事の種類の取得
EJobType CJobStatusBase::GetJobType()
{
	return mJobType;
}
