#include "CSwitchMoveWall.h"
#include "CColliderMesh.h"
#include "Maths.h"

// コンストラクタ
CSwitchMoveWall::CSwitchMoveWall(CModel* model, CModel* colModel,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime)
	: mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
{
	mpModel = model;

	mpCol = new CColliderMesh(this, ELayer::eWall, colModel, true);

	Position(mDefaultPos);
	Scale(scale);
}

// デストラクタ
CSwitchMoveWall::~CSwitchMoveWall()
{
}

// 作用していない時の処理
void CSwitchMoveWall::UpdateOff()
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
void CSwitchMoveWall::UpdateOn()
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
void CSwitchMoveWall::Move()
{
	// 時間が進んだ割合
	float per = mElapsedTime / mMoveTime;
	// 座標を求める
	Position(mDefaultPos + mMoveVec * per);
}
