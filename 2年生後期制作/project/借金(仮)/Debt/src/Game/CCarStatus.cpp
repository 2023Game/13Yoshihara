#include "CCarStatus.h"

// 使わないベースステータスは値が0
#define MAX_HP 0						// 最大HP
#define MOVE_SPEED 0.375f * 5.0f		// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define KNOCKBACK_SPEED 0.375f * 5.0f	// ノックバック速度
#define POWER 1							// ゴミを落とすパワー

#define POP_TIME 10.0f		// 出現までの時間
#define DELETE_TIME 2.0f	// 消滅までの時間

CCarStatus::CCarStatus()
	: CTrashStatusBase()
{
	// ベースステータスを設定
	mStatus =
	{
		MAX_HP,
		MOVE_SPEED,
		JUMP_SPEED,
		KNOCKBACK_SPEED,
		POWER,
	};

	// 出現までの時間
	mPopTime = POP_TIME;
	// 消滅までの時間
	mDeleteTime = DELETE_TIME;
}

CCarStatus::~CCarStatus()
{
}
