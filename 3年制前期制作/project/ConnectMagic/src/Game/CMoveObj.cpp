#include "CMoveObj.h"
#include "Maths.h"
#include "CConnectPointManager.h"

// 一時停止の時間
#define STOP_TIME 0.5f

CMoveObj::CMoveObj(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale,
	const CVector& move, float moveTime,
	ELayer layer, bool isCrushed)
	: mpModel(model)
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mState(EMoveState::eStop)
	, mPreState(EMoveState::eBack)
	, mpCrushedCol(nullptr)
{
	// 本体のコライダー
	mpColliderMesh = new CColliderMesh(this, layer, mpModel, true);
	// プレイヤーを壊すなら
	if (isCrushed)
	{
		// プレイヤーが挟まれた時用のコライダー
		mpCrushedCol = new CColliderMesh(this, ELayer::eCrushed, col, true);
		// プレイヤーとだけ衝突
		mpCrushedCol->SetCollisionLayers({ ELayer::ePlayer });
	}

	// 接続部の管理クラス
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// コライダーを追加
	pointMgr->AddCollider(mpColliderMesh);
	// 現在のカメラ
	CCamera* camera = CCamera::CurrentCamera();
	// コライダーを追加
	camera->AddCollider(mpColliderMesh);

	Position(mDefaultPos);
	Scale(scale);
}

CMoveObj::~CMoveObj()
{
	SAFE_DELETE(mpColliderMesh);
	SAFE_DELETE(mpCrushedCol);
}

void CMoveObj::Update()
{
	mElapsedTime += Times::DeltaTime();

	switch (mState)
	{
	case EMoveState::eStop:
		UpdateStop();
		break;
	case EMoveState::eGo:
		UpdateGo();
		break;
	case EMoveState::eBack:
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

// 状態を設定
void CMoveObj::SetState(EMoveState state)
{
	mState = state;
}

// 状態を取得
EMoveState CMoveObj::GetState() const
{
	return mState;
}

// 前回の状態を設定
void CMoveObj::SetPreState(EMoveState state)
{
	mPreState = state;
}

// 前回の状態を取得
EMoveState CMoveObj::GetPreState() const
{
	return mPreState;
}

// 経過時間を設定
void CMoveObj::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// 経過時間を取得
float CMoveObj::GetElapsedTime() const
{
	return mElapsedTime;
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
