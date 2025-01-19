#include "CGarbageTruckStatus.h"

// 使わないベースステータスは値が0
// 通常のゴミ収集車のステータス
#define NORMAL_MAX_HP 3									// 最大HP
#define NORMAL_BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// 移動速度
#define NORMAL_JUMP_SPEED 0.0f							// ジャンプ速度
#define NORMAL_KNOCKBACK_SPEED 0.375f * 5.0f			// ノックバック速度
#define NORMAL_ATTACK_POWER 1							// 攻撃力
#define NORMAL_POWER 1									// ゴミを落とすパワー
#define NORMAL_WITHDRAW_TIME 10.0f						// 撤退までの時間
#define NORMAL_COLLECTORS 3								// 回収員の人数
// お仕置きのゴミ収集車のステータス
#define PUNISHER_MAX_HP 3								// 最大HP
#define PUNISHER_BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// 移動速度
#define PUNISHER_JUMP_SPEED 0.0f						// ジャンプ速度
#define PUNISHER_KNOCKBACK_SPEED 0.375f * 5.0f			// ノックバック速度
#define PUNISHER_ATTACK_POWER 1							// 攻撃力
#define PUNISHER_POWER 1								// ゴミを落とすパワー
#define PUNISHER_WITHDRAW_TIME 10.0f					// 撤退までの時間
#define PUNISHER_COLLECTORS 3							// 回収員の人数

CGarbageTruckStatus::CGarbageTruckStatus(bool punisher)
	: mIsPunisher(punisher)
{
	// お仕置き用なら
	if (punisher)
	{
		// お仕置き用のステータス
		CVehicleStatus(PUNISHER_MAX_HP, PUNISHER_BASE_MOVE_SPEED, PUNISHER_JUMP_SPEED,
			PUNISHER_KNOCKBACK_SPEED, PUNISHER_ATTACK_POWER, PUNISHER_POWER);
		mWithdrawTime = PUNISHER_WITHDRAW_TIME;
		mCollectors = PUNISHER_COLLECTORS;
	}
	// 通常用なら
	else
	{
		// 通常のステータス
		CVehicleStatus(NORMAL_MAX_HP, NORMAL_BASE_MOVE_SPEED, NORMAL_JUMP_SPEED,
			NORMAL_KNOCKBACK_SPEED, NORMAL_ATTACK_POWER, NORMAL_POWER);
		mWithdrawTime = NORMAL_WITHDRAW_TIME;
		mCollectors = NORMAL_COLLECTORS;
	}
}

CGarbageTruckStatus::~CGarbageTruckStatus()
{
}
// 撤退までの時間を取得
float CGarbageTruckStatus::GetBackTime() const
{
	return mWithdrawTime;
}

// 撤退までの時間を初期値に設定
void CGarbageTruckStatus::SetWithdrawTime()
{
	// お仕置き用
	if (mIsPunisher)
	{
		mWithdrawTime = PUNISHER_WITHDRAW_TIME;
	}
	// 通常用
	else
	{
		mWithdrawTime = NORMAL_WITHDRAW_TIME;
	}
}

// 撤退までの時間が経過したかどうか
bool CGarbageTruckStatus::IsElapsedWithdrawTime() const
{
	// 撤退までの時間が0より大きいなら経過していない
	if (mWithdrawTime > 0.0f)
	{
		return false;
	}
	// 0以下なら経過した
	else
	{
		return true;
	}
}

// 撤退までの時間をカウントダウン
void CGarbageTruckStatus::CountWithdrawTime()
{
	mWithdrawTime -= Times::DeltaTime();
}

// 回収員の人数を取得する
int CGarbageTruckStatus::GetCollectors() const
{
	return mCollectors;
}

// 回収員の人数を初期値に設定する
void CGarbageTruckStatus::SetCollectors()
{
	// お仕置き用
	if (mIsPunisher)
	{
		mCollectors = PUNISHER_COLLECTORS;
	}
	// 通常用
	else
	{
		mCollectors = NORMAL_COLLECTORS;
	}
}

// 回収員の人数を1減らす
void CGarbageTruckStatus::DecreaseCollectors()
{
	mCollectors--;
}


