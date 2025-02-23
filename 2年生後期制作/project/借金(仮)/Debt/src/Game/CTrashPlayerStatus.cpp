#include "CTrashPlayerStatus.h"

#define MAX_HP 10								// 最大HP
#define BASE_MOVE_SPEED 0.75f * 60.0f			// 移動速度
#define JUMP_SPEED 1.5f							// ジャンプ速度
#define KNOCKBACK_SPEED 0.75f * 60.0f * 2.0f	// ノックバック速度
#define ATTACK_POWER 1							// 攻撃力
#define POWER 1									// ゴミを落とすパワー
#define CRITICAL_CHANCE 10						// クリティカル確率

// コンストラクタ
CTrashPlayerStatus::CTrashPlayerStatus()
	: CTrashStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED, 
		KNOCKBACK_SPEED, ATTACK_POWER, POWER)
	, mCriticalChance(CRITICAL_CHANCE)
{

}

// デストラクタ
CTrashPlayerStatus::~CTrashPlayerStatus()
{
}

// クリティカル確率を取得
int CTrashPlayerStatus::GetCriticalChance()
{
	return mCriticalChance;
}


