#include "CSwitchMoveAirObj.h"
#include "Maths.h"
#include "CAirConnectObj.h"

// 移動速度
#define MOVE_SPEED 20.0f
// 近い距離
#define NEAR_DIST 1.0f
// 一時停止の時間
#define STOP_TIME 0.5f

// 状態を設定
void CSwitchMoveAirObj::SetState(EMoveState state)
{
	mMoveState = state;
}

// 状態を取得
EMoveState CSwitchMoveAirObj::GetState() const
{
	return mMoveState;
}

// 経過時間を設定
void CSwitchMoveAirObj::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// 経過時間を取得
float CSwitchMoveAirObj::GetElapsedTime() const
{
	return mElapsedTime;
}

// コンストラクタ
CSwitchMoveAirObj::CSwitchMoveAirObj(
	const CVector& pos,
	std::vector<CVector> targets)
	: CSwitchObject()
	, mMoveState(EMoveState::eStop)
	, mElapsedTime(0.0f)
	, mpConnectObj(nullptr)
	, mTargetPos(targets)
	, mTargetPosNum(0)
{
	// 座標を設定
	Position(pos);
	// 空中接続オブジェクトを生成
	mpConnectObj = new CAirConnectObj();
	mpConnectObj->Position(pos);
}

// デストラクタ
CSwitchMoveAirObj::~CSwitchMoveAirObj()
{
}

// 空中オブジェクトを取得
CAirConnectObj* CSwitchMoveAirObj::GetAirConnectObj()
{
	return mpConnectObj;
}

// 有効無効の切り替え
void CSwitchMoveAirObj::SetEnable(bool enable)
{
	CTask::SetEnable(enable);
	mpConnectObj->SetEnable(enable);
}

// 作用していないときの処理
void CSwitchMoveAirObj::UpdateOff()
{
}

// 作用しているときの処理
void CSwitchMoveAirObj::UpdateOn()
{
	Move();
}

// 移動の処理
void CSwitchMoveAirObj::Move()
{
	// 今の座標
	CVector pos = Position();
	// 目標座標
	CVector targetPos = mTargetPos[mTargetPosNum];

	// 移動方向
	CVector dir = targetPos - pos;
	// 距離
	float dist = dir.Length();

	// 到達した
	if (dist < NEAR_DIST)
	{
		mElapsedTime += Times::DeltaTime();
		// 一時停止の時間が経過したら
		if (mElapsedTime >= STOP_TIME)
		{
			mElapsedTime = 0.0f;
			// 座標を目標座標に設定
			Position(targetPos);
			// 要素番号を次へ
			mTargetPosNum++;
			// 要素数以上なら最初へ
			if (mTargetPosNum >= mTargetPos.size())
			{
				mTargetPosNum = 0;
			}
		}
	}
	// 到達していない
	else
	{
		dir.Normalize();
		// 新しい座標を計算
		CVector newPos = pos + dir * MOVE_SPEED * Times::DeltaTime();
		Position(newPos);
		mpConnectObj->Position(newPos);
	}
}

// 状態を変更
void CSwitchMoveAirObj::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;

	// 状態を変更
	mMoveState = state;
	mElapsedTime = 0.0f;
}
