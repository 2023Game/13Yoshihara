#include "CMoveObj.h"
#include "Maths.h"

// 一時停止の時間
#define STOP_TIME 0.5f

CMoveObj::CMoveObj(CModel* model, 
	const CVector& pos, const CVector& scale,
	const CVector& move, float moveTime,
	ELayer layer)
	: mpModel(model)
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mState(EMoveState::eStop)
	, mPreState(EMoveState::eBack)
{
	mpColliderMesh = new CColliderMesh(this, layer, mpModel, true);

	Position(mDefaultPos);
	Scale(scale);
}

CMoveObj::~CMoveObj()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CMoveObj::Update()
{
	mElapsedTime += Times::DeltaTime();

	switch (mState)
	{
	case CMoveObj::EMoveState::eStop:
		UpdateStop();
		break;
	case CMoveObj::EMoveState::eGo:
		UpdateGo();
		break;
	case CMoveObj::EMoveState::eBack:
		UpdateBack();
		break;
	}
}

void CMoveObj::Render()
{
	mpModel->Render(Matrix());
}

// 状態を変更
void CMoveObj::ChangeState(EMoveState state)
{
	if (mState == state) return;
	
	// 一つ前の状態を更新
	mPreState = mState;
	// 状態を変更
	mState = state;
	mElapsedTime = 0.0f;
}

// 止まっているときの更新
void CMoveObj::UpdateStop()
{
	// 一時停止の時間が経過したら
	if (mElapsedTime >= STOP_TIME)
	{
		// 一つ前が進む状態だったら
		if (mPreState == EMoveState::eGo)
		{
			// 戻る状態に変更
			ChangeState(EMoveState::eBack);
		}
		// 戻る状態なら
		else if (mPreState == EMoveState::eBack)
		{
			// 進む状態へ
			ChangeState(EMoveState::eGo);
		}
	}
}

// 進んでいるときの更新
void CMoveObj::UpdateGo()
{
	float per = mElapsedTime / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// 時間が経過したら一時停止状態へ
	if (mElapsedTime >= mMoveTime)
	{
		ChangeState(EMoveState::eStop);
	}
}

// 戻っているときの更新
void CMoveObj::UpdateBack()
{
	float per = (mMoveTime - mElapsedTime) / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// 時間が経過したら一時停止状態へ
	if (mElapsedTime >= mMoveTime)
	{
		ChangeState(EMoveState::eStop);
	}
}
