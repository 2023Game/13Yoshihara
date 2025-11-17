#include "CSwitchPushWall.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"

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
	, mMoveState(EMoveState::eGo)
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
	switch (mMoveState)
	{
	case EMoveState::eGo:
		mElapsedTime += Times::DeltaTime();
		break;
	case EMoveState::eBack:
		mElapsedTime -= Times::DeltaTime();
		break;
	}
	if (mElapsedTime <= 0.0f)
	{
		SetState(EMoveState::eGo);
		SetElapsedTime(0.0f);
	}
	else if (mElapsedTime >= mMoveTime)
	{
		SetState(EMoveState::eBack);
		SetElapsedTime(mMoveTime);
	}

	Move();
}

// スイッチがオンになったら動かない
void CSwitchPushWall::UpdateOn()
{
}

void CSwitchPushWall::Move()
{
	// 時間が進んだ割合
	float per = mElapsedTime / mMoveTime;
	// 座標を求める
	Position(mDefaultPos + mMoveVec * per);
}

void CSwitchPushWall::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;

	// 状態を変更
	mMoveState = state;
	mElapsedTime = 0.0f;
}
