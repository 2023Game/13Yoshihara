#include "CSwitchMoveFloor.h"
#include "CColliderMesh.h"

// コンストラクタ
CSwitchMoveFloor::CSwitchMoveFloor(CModel* model, CModel* colModel,
	CVector& pos, CVector& move, float moveTime)
	: CSwitchObject()
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
{
	mpModel = model;

	mpCol = new CColliderMesh(this, ELayer::eGround, colModel, true);

	Position(mDefaultPos);
}

// デストラクタ
CSwitchMoveFloor::~CSwitchMoveFloor()
{
}

// 作用していない時の処理
void CSwitchMoveFloor::UpdateOff()
{	
	// 経過時間が0秒なら処理しない
	if (mElapsedTime == 0.0f) return;

	// 移動処理
	Move();

	// 経過時間を減少
	mElapsedTime -= Times::DeltaTime();
	// 経過時間が0以下なら
	if (mElapsedTime <= 0.0f)
	{
		// 経過時間は0
		mElapsedTime = 0.0f;
	}
}

// 作用している時の処理
void CSwitchMoveFloor::UpdateOn()
{
	// 移動時間が経過していたら処理しない
	if (mElapsedTime == mMoveTime) return;

	// 移動処理
	Move();

	// 時間経過
	mElapsedTime += Times::DeltaTime();
	// 移動時間が経過していたら
	if (mElapsedTime >= mMoveTime)
	{
		// 経過時間の値を移動時間にする
		mElapsedTime = mMoveTime;
	}
}

// 移動の処理
void CSwitchMoveFloor::Move()
{
	// 時間が進んだ割合
	float per = mElapsedTime / mMoveTime;
	// 座標を求める
	Position(mDefaultPos + mMoveVec * per);
}
