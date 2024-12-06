#include "CCarStatus.h"

// 使わないベースステータスは値が0
#define MAX_HP 0						// 最大HP
#define MOVE_SPEED 0.375f * 5.0f		// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define KNOCKBACK_SPEED 0.375f * 5.0f	// ノックバック速度
#define POWER 1							// ゴミを落とすパワー

#define DELETE_TIME 2.0f	// 消滅までの時間

CCarStatus::CCarStatus()
	: CTrashStatusBase()
{
	
	// ベースステータスを設定
	SetStatusBase(
		MAX_HP,
		MOVE_SPEED,
		JUMP_SPEED,
		KNOCKBACK_SPEED,
		POWER
		);

	// 消滅までの時間を設定
	SetDeleteTime(DELETE_TIME);
}

CCarStatus::~CCarStatus()
{
}

// 消滅までの時間を取得する
float CCarStatus::GetDeleteTime()
{
	return mDeleteTime;
}

// 消滅までの時間を設定する
void CCarStatus::SetDeleteTime(float deleteTime)
{
	mDeleteTime = deleteTime;
}

// 消滅までの時間が経過したかどうか
bool CCarStatus::IsElapsedDeleteTime()
{
	// 消滅までの時間が0より大きいなら経過していない
	if (mDeleteTime > 0.0f)
	{
		return false;
	}
	// 0以下なら経過した
	else
	{
		return true;
	}
}

// 消滅までの時間をカウント
void CCarStatus::CountDeleteTime()
{
	// 0より大きいなら減算
	if (mDeleteTime > 0.0f)
	{
		mDeleteTime -= Times::DeltaTime();
	}
}


