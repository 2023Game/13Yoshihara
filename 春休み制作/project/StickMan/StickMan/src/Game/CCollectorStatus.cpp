#include "CCollectorStatus.h"

// 通常用の回収員ステータス
#define NORMAL_MAX_HP 1									// 最大HP
#define NORMAL_BASE_MOVE_SPEED 0.375f * 2.0f * 60.0f	// 移動速度
#define NORMAL_JUMP_SPEED 0.2f							// ジャンプ速度
#define NORMAL_KNOCKBACK_SPEED 0.375f * 2.0f			// ノックバック速度
#define NORMAL_ATTACK_POWER 0							// 攻撃力
#define NORMAL_POWER 1									// ゴミを落とすパワー
// お仕置き用の回収員ステータス
#define PUNISHER_MAX_HP 1								// 最大HP
#define PUNISHER_BASE_MOVE_SPEED 0.375f * 2.0f * 60.0f	// 移動速度
#define PUNISHER_JUMP_SPEED 0.2f						// ジャンプ速度
#define PUNISHER_KNOCKBACK_SPEED 0.375f * 2.0f			// ノックバック速度
#define PUNISHER_ATTACK_POWER 0							// 攻撃力
#define PUNISHER_POWER 2								// ゴミを落とすパワー

// コンストラクタ
CCollectorStatus::CCollectorStatus(bool punisher)
	: CTrashStatusBase(NORMAL_MAX_HP, NORMAL_BASE_MOVE_SPEED, NORMAL_JUMP_SPEED,
		NORMAL_KNOCKBACK_SPEED, NORMAL_ATTACK_POWER, NORMAL_POWER)
	, mIsPunisher(punisher)
{
	// お仕置き用なら
	if (punisher)
	{
		// お仕置き用のステータスに変更する
		SetAllStatus(PUNISHER_MAX_HP, PUNISHER_BASE_MOVE_SPEED, PUNISHER_JUMP_SPEED,
			PUNISHER_KNOCKBACK_SPEED, PUNISHER_ATTACK_POWER, PUNISHER_POWER);
	}
}

// デストラクタ
CCollectorStatus::~CCollectorStatus()
{
}

// お仕置き用かどうかを取得する
bool CCollectorStatus::GetPunisher() const
{
	return mIsPunisher;
}
