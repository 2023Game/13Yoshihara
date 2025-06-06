#include "CPlayerStatus.h"

#define MAX_HP 5				// 最大体力
#define MOVE_SPEED 2.0f * 60.0f	// 移動速度
#define JUMP_SPEED 1.2f * 60.0f	// ジャンプ力
#define ATTACK_POWER 0			// 攻撃力


// コンストラクタ
CPlayerStatus::CPlayerStatus()
	: CCharaStatusBase(MAX_HP, MOVE_SPEED, JUMP_SPEED, ATTACK_POWER)
{
}

// デストラクタ
CPlayerStatus::~CPlayerStatus()
{
}
