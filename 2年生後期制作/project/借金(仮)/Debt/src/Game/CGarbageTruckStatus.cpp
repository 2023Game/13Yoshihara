#include "CGarbageTruckStatus.h"

// 使わないベースステータスは値が0
#define MAX_HP 3						// 最大HP
#define MOVE_SPEED 0.375f * 5.0f		// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define KNOCKBACK_SPEED 0.375f * 5.0f	// ノックバック速度
#define ATTACK_POWER 1					// 攻撃力
#define POWER 1							// ゴミを落とすパワー

#define BACK_TIME 10.0f		// 撤退までの時間
#define COLLECT_RADIUS 5.0f	// 回収範囲の半径
#define COLLECTORS 3		// 回収員の人数

CGarbageTruckStatus::CGarbageTruckStatus()
	: CVehicleStatus(MAX_HP, MOVE_SPEED, JUMP_SPEED,
		KNOCKBACK_SPEED, ATTACK_POWER, POWER)
	, mBackTime(BACK_TIME)
	, mCollectRadius(COLLECT_RADIUS)
	, mCollectors(COLLECTORS)
{
}

CGarbageTruckStatus::~CGarbageTruckStatus()
{
}
// 撤退までの時間を取得
float CGarbageTruckStatus::GetBackTime() const
{
	return mBackTime;
}

// 撤退までの時間を初期値に設定
void CGarbageTruckStatus::SetBackTime()
{
	mBackTime = BACK_TIME;
}

// 撤退までの時間が経過したかどうか
bool CGarbageTruckStatus::IsElapsedBackTime() const
{
	// 撤退までの時間が0より大きいなら経過していない
	if (mBackTime > 0.0f)
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
void CGarbageTruckStatus::CountBackTime()
{
	mBackTime -= Times::DeltaTime();
}

// 回収範囲の半径を取得する
float CGarbageTruckStatus::GetCollectRadius() const
{
	return mCollectRadius;
}

// 回収員の人数を取得する
int CGarbageTruckStatus::GetCollectors() const
{
	return mCollectors;
}

// 回収員の人数を初期値に設定する
void CGarbageTruckStatus::SetCollectors()
{
	mCollectors = COLLECTORS;
}

// 回収員の人数を1減らす
void CGarbageTruckStatus::DecreaseCollectors()
{
	mCollectors--;
}


