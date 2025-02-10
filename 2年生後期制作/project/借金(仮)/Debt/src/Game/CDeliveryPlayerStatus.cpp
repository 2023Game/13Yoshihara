#include "CDeliveryPlayerStatus.h"

#define MAX_HP 10						// 最大HP
#define BASE_MOVE_SPEED 7.5f * 60.0f	// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define ATTACK_POWER 1					// 攻撃力

// コンストラクタ
CDeliveryPlayerStatus::CDeliveryPlayerStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, ATTACK_POWER)
{
}

// デストラクタ
CDeliveryPlayerStatus::~CDeliveryPlayerStatus()
{
}
