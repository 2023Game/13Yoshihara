#include "CCarStatus.h"

#define MAX_HP 0						// 最大HP（未使用）
#define BASE_MOVE_SPEED 0.375f * 5.0f	// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度（未使用）
#define KNOCKBACK_SPEED 0.375f * 5.0f	// ノックバック速度
#define POWER 1							// ゴミを落とすパワー

#define DELETE_TIME 2.0f	// 消滅までの時間

// コンストラクタ
CCarStatus::CCarStatus()
	: CTrashStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, KNOCKBACK_SPEED, POWER)
	, mDeleteTime(DELETE_TIME)
{
}

/*
コンストラクタ
このクラスを継承したキャラのステータスの設定用
*/
CCarStatus::CCarStatus(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockback, int power)
	: CTrashStatusBase(maxHp, baseMoveSpeed,
		jumpSpeed, knockback, power)
	, mDeleteTime(DELETE_TIME)
{
}

CCarStatus::~CCarStatus()
{
}

// 消滅までの時間を取得する
float CCarStatus::GetDeleteTime() const
{
	return mDeleteTime;
}

// 消滅までの時間を初期値に設定する
void CCarStatus::SetDeleteTime()
{
	mDeleteTime = DELETE_TIME;
}

// 消滅までの時間が経過したかどうか
bool CCarStatus::IsElapsedDeleteTime() const
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

// 消滅までの時間をカウントダウン
void CCarStatus::CountDeleteTime()
{
	// 0より大きいなら減算
	if (mDeleteTime > 0.0f)
	{
		mDeleteTime -= Times::DeltaTime();
	}
}


