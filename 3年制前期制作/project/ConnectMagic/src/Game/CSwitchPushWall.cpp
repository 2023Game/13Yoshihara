#include "CSwitchPushWall.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"
#include "Maths.h"

// 一時停止の時間
#define STOP_TIME 0.5f

void CSwitchPushWall::SetState(EMoveState state)
{
	mMoveState = state;
}

EMoveState CSwitchPushWall::GetState() const
{
	return mMoveState;
}

void CSwitchPushWall::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

float CSwitchPushWall::GetElapsedTime() const
{
	return mElapsedTime;
}

CSwitchPushWall::CSwitchPushWall(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime, bool isCrushed)
	: CSwitchObject(ETaskPriority::eBackground)
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mpCrushedCol(nullptr)
	, mMoveState(EMoveState::eStop)
	, mPreMoveState(EMoveState::eBack)
{
	mpModel = model;

	// コライダー生成
	CreateCol();
	// プレイヤーを壊すなら
	if (isCrushed)
	{
		// プレイヤーが挟まれた時用のコライダー
		mpCrushedCol = new CColliderMesh(this, ELayer::eCrushed, col, true);
		// プレイヤーとだけ衝突
		mpCrushedCol->SetCollisionLayers({ ELayer::ePlayer });
	}

	Position(mDefaultPos);
	Scale(scale);
}

CSwitchPushWall::~CSwitchPushWall()
{
	SAFE_DELETE(mpCrushedCol);
}

void CSwitchPushWall::CreateCol()
{
	mpCol = new CColliderMesh(this, ELayer::eWall, mpModel, true);

	// 接続部の管理クラス
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// コライダーを追加
	pointMgr->AddCollider(mpCol);
	// 現在のカメラ
	CCamera* camera = CCamera::CurrentCamera();
	camera->AddCollider(mpCol);
}

void CSwitchPushWall::UpdateOff()
{
	mElapsedTime += Times::DeltaTime();

	switch (mMoveState)
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

// スイッチがオンになったら動かない
void CSwitchPushWall::UpdateOn()
{
}

// 止まっているときの更新
void CSwitchPushWall::UpdateStop()
{
	// 一時停止の時間が経過したら
	if (mElapsedTime >= STOP_TIME)
	{
		// 一つ前が進む状態だったら
		if (mPreMoveState == EMoveState::eGo)
		{
			// 戻る状態に変更
			ChangeMoveState(EMoveState::eBack);
		}
		// 戻る状態なら
		else if (mPreMoveState == EMoveState::eBack)
		{
			// 進む状態へ
			ChangeMoveState(EMoveState::eGo);
		}
	}
}

// 進んでいるときの更新
void CSwitchPushWall::UpdateGo()
{
	float per = mElapsedTime / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// 時間が経過したら一時停止状態へ
	if (mElapsedTime >= mMoveTime)
	{
		ChangeMoveState(EMoveState::eStop);
	}
}

// 戻っているときの更新
void CSwitchPushWall::UpdateBack()
{
	float per = (mMoveTime - mElapsedTime) / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// 時間が経過したら一時停止状態へ
	if (mElapsedTime >= mMoveTime)
	{
		ChangeMoveState(EMoveState::eStop);
	}
}

void CSwitchPushWall::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;

	// ひとつ前の状態を更新
	mPreMoveState = mMoveState;
	// 状態を変更
	mMoveState = state;
	mElapsedTime = 0.0f;
}
