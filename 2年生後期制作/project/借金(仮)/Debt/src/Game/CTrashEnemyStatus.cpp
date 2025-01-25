#include "CTrashEnemyStatus.h"

// 通常用のゴミ拾い敵ステータス
#define NORMAL_MAX_HP 10								// 最大HP
#define NORMAL_BASE_MOVE_SPEED 0.375f * 2.0f * 20.0f	// 移動速度
#define NORMAL_JUMP_SPEED 1.5f							// ジャンプ速度
#define NORMAL_KNOCKBACK_SPEED 0.375f * 2.0f			// ノックバック速度
#define NORMAL_ATTACK_POWER 1							// 攻撃力
#define NORMAL_POWER 1									// ゴミを落とすパワー
#define NORMAL_CRITICAL_CHANCE 10						// クリティカル確率
// お仕置き用のゴミ拾い敵ステータス
#define PUNISHER_MAX_HP 10								// 最大HP
#define PUNISHER_BASE_MOVE_SPEED 0.375f * 2.0f * 20.0f	// 移動速度
#define PUNISHER_JUMP_SPEED 1.5f						// ジャンプ速度
#define PUNISHER_KNOCKBACK_SPEED 0.375f * 2.0f			// ノックバック速度
#define PUNISHER_ATTACK_POWER 1							// 攻撃力
#define PUNISHER_POWER 1								// ゴミを落とすパワー
#define PUNISHER_CRITICAL_CHANCE 10						// クリティカル確率

// コンストラクタ
CTrashEnemyStatus::CTrashEnemyStatus(bool punisher)
	: CTrashStatusBase(NORMAL_MAX_HP, NORMAL_BASE_MOVE_SPEED, NORMAL_JUMP_SPEED,
		NORMAL_KNOCKBACK_SPEED, NORMAL_ATTACK_POWER, NORMAL_POWER)
	, mCriticalChance(NORMAL_CRITICAL_CHANCE)
	, mIsPunisher(punisher)
{
	// お仕置き用なら
	if (punisher)
	{
		// お仕置き用のステータスに変更する
		SetAllStatus(PUNISHER_MAX_HP, PUNISHER_BASE_MOVE_SPEED, PUNISHER_JUMP_SPEED,
			PUNISHER_KNOCKBACK_SPEED, PUNISHER_ATTACK_POWER, PUNISHER_POWER);
		mCriticalChance = PUNISHER_CRITICAL_CHANCE;
	}
}

// デストラクタ
CTrashEnemyStatus::~CTrashEnemyStatus()
{
}

// クリティカル確率を取得
int CTrashEnemyStatus::GetCriticalChance()
{
	return mCriticalChance;
}

// クリティカル確率を設定
void CTrashEnemyStatus::SetCriticalChance(int criticalChance)
{
	mCriticalChance = criticalChance;
}
