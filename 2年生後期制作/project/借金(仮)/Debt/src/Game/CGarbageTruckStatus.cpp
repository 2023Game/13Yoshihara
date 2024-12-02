#include "CGarbageTruckStatus.h"

// 使わないベースステータスは値が0
#define MAX_HP 3						// 最大HP
#define MOVE_SPEED 0.375f * 5.0f		// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define KNOCKBACK_SPEED 0.375f * 5.0f	// ノックバック速度
#define POWER 1							// ゴミを落とすパワー

#define POP_TIME 20.0f		// 出現までの時間
#define DELETE_TIME 2.0f	// 消滅までの時間
#define BACK_TIME 10.0f		// 撤退までの時間
#define COLLECT_RADIUS 5.0f	// 回収範囲の半径
#define COLLECTORS 3		// 回収員の人数

CGarbageTruckStatus::CGarbageTruckStatus()
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
	// 撤退までの時間
	mBackTime = BACK_TIME;
	// 回収範囲の半径
	mCollectRadius = COLLECT_RADIUS;
	// 回収員の人数
	mCollectors = COLLECTORS;
}

CGarbageTruckStatus::~CGarbageTruckStatus()
{
}
