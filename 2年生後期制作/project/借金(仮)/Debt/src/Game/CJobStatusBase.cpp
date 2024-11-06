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

// �A�����b�N��Ԃ��擾
bool CJobStatusBase::GetUnlock()
{
	return mUnlock;
}

// �A�����b�N��Ԃ�ݒ�
void CJobStatusBase::SetUnlock(bool unlock)
{
	mUnlock = unlock;
}

// �����{�����擾
float CJobStatusBase::GetUpgrade()
{
	return mUpgrade;
}

// �����{����ݒ�
void CJobStatusBase::SetUpgrade(float upgrade)
{
	mUpgrade = mUpgrade + upgrade;
}

// �d���̎�ނ̎擾
EJobType CJobStatusBase::GetJobType()
{
	return mJobType;
}
