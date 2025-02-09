#include "CDeliveryEnemyStatus.h"

#define MAX_HP 1						// 最大HP
#define BASE_MOVE_SPEED 7.5f * 60.0f	// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define ATTACK_POWER 0					// 攻撃力
#define SCORE 1000	// 得点

// コンストラクタ
CDeliveryEnemyStatus::CDeliveryEnemyStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, ATTACK_POWER)
	, mScore(SCORE)
{
}

// デストラクタ
CDeliveryEnemyStatus::~CDeliveryEnemyStatus()
{
}

// 壊したら貰える得点を取得
int CDeliveryEnemyStatus::GetScore() const
{
	return mScore;
}
