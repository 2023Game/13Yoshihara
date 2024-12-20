#include "CTrashPlayerStatus.h"

#define MAX_HP 3						// 最大HP
#define MOVE_SPEED 0.375f * 2.0f		// 移動速度
#define JUMP_SPEED 1.5f					// ジャンプ速度
#define KNOCKBACK_SPEED 0.375f * 2.0f	// ノックバック速度
#define POWER 1							// ゴミを落とすパワー

CTrashPlayerStatus::CTrashPlayerStatus()
	: CTrashStatusBase(MAX_HP, MOVE_SPEED, JUMP_SPEED, KNOCKBACK_SPEED, POWER)
{
}

CTrashPlayerStatus::~CTrashPlayerStatus()
{
}


