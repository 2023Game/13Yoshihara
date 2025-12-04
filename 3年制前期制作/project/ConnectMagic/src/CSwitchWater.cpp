#include "CSwitchWater.h"
#include "CWater.h"

#define MOVE_TIME 1.0f

CSwitchWater::CSwitchWater(const CVector& pos, const CVector& downPos, const CVector& scale)
	: CSwitchObject(ETaskPriority::eBackground)
	, mDefaultPos(pos)
	, mDownPos(downPos)
	, mElapsedTime(0.0f)
{
	mpWater = new CWater(scale);
}

CSwitchWater::~CSwitchWater()
{
}

// 作用していないときの更新
void CSwitchWater::UpdateOff()
{
	mElapsedTime += Times::DeltaTime();

	if (MOVE_TIME <= mElapsedTime)
	{
		Position(mDefaultPos);
		mElapsedTime = MOVE_TIME;
	}
	else
	{
		// 進行度
		float per = mElapsedTime / MOVE_TIME;
		// 進行度で補間する
		CVector::Lerp(mDownPos, mDefaultPos, per);
	}
}

// 作用しているときの更新
void CSwitchWater::UpdateOn()
{
	mElapsedTime += Times::DeltaTime();

	if (MOVE_TIME <= mElapsedTime)
	{
		Position(mDownPos);
		mElapsedTime = MOVE_TIME;
	}
	else
	{
		// 進行度
		float per = mElapsedTime / MOVE_TIME;
		// 進行度で補間する
		CVector::Lerp(mDefaultPos, mDownPos, per);
	}
}

void CSwitchWater::SetEnable(bool enable)
{
	CTask::SetEnable(enable);
	mpWater->SetEnable(enable);
}

void CSwitchWater::ChangeState(EState state)
{
	if (mState != state)
	{
		mState = state;
		mElapsedTime = 0.0f;
	}
}
