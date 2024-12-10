#include "CGarbageTruckStatus.h"

// 使わないベースステータスは値が0
#define MAX_HP 3						// 最大HP
#define MOVE_SPEED 0.375f * 5.0f		// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define KNOCKBACK_SPEED 0.375f * 5.0f	// ノックバック速度
#define POWER 1							// ゴミを落とすパワー

#define BACK_TIME 10.0f		// 撤退までの時間
#define COLLECT_RADIUS 5.0f	// 回収範囲の半径
#define COLLECTORS 3		// 回収員の人数

CGarbageTruckStatus::CGarbageTruckStatus()
{
	// 回収範囲の半径を設定
	mCollectRadius = COLLECT_RADIUS;
	// 回収員の人数を設定
	SetCollectors(COLLECTORS);
}

CGarbageTruckStatus::~CGarbageTruckStatus()
{
}
// 撤退までの時間を取得
float CGarbageTruckStatus::GetBackTime()
{
	return mBackTime;
}

// 撤退までの時間を設定
void CGarbageTruckStatus::SetBackTime(float backTime)
{
	mBackTime = backTime;
}

// 撤退までの時間をカウント
void CGarbageTruckStatus::CountBackTime()
{
	mBackTime -= Times::DeltaTime();
}

// 回収範囲の半径を取得する
float CGarbageTruckStatus::GetCollectRadius()
{
	return mCollectRadius;
}

// 回収員の人数を取得する
int CGarbageTruckStatus::GetCollectors()
{
	return mCollectors;
}

// 回収員の人数を設定する
void CGarbageTruckStatus::SetCollectors(int collectors)
{
	mCollectors = collectors;
}
