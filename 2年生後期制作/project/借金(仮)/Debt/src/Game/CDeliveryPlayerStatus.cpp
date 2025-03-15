#include "CDeliveryPlayerStatus.h"

#define MAX_HP 10						// 最大HP
#define BASE_MOVE_SPEED 7.5f * 60.0f	// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define ATTACK_POWER 1					// 攻撃力
#define THROW_SPEED 7.5f * 60.0f * 2.0f	// 投げる速度

// Hp一つ当たりの移動速度の減少値
#define SPEED_DOWN 7.5f

// コンストラクタ
CDeliveryPlayerStatus::CDeliveryPlayerStatus()
	: CDeliveryStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, ATTACK_POWER, THROW_SPEED)
{
}

// デストラクタ
CDeliveryPlayerStatus::~CDeliveryPlayerStatus()
{
}