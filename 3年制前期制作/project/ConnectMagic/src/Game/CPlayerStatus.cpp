#include "CPlayerStatus.h"

constexpr int MAX_HP =		 5;				// 最大体力
constexpr float MOVE_SPEED = 1.0f * 60.0f;	// 移動速度
constexpr float JUMP_SPEED = 1.2f * 60.0f;	// ジャンプ力
constexpr int ATTACK_POWER = 1;				// 攻撃力


// コンストラクタ
CPlayerStatus::CPlayerStatus()
	: CCharaStatusBase(MAX_HP, MOVE_SPEED, JUMP_SPEED, ATTACK_POWER)
{
}

// デストラクタ
CPlayerStatus::~CPlayerStatus()
{
}
