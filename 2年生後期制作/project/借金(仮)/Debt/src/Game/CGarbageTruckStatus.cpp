#include "CGarbageTruckStatus.h"

// 使わないベースステータスは値が0
// 通常のゴミ収集車のステータス
#define NORMAL_MAX_HP 30								// 最大HP
#define NORMAL_BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// 移動速度
#define NORMAL_JUMP_SPEED 0.0f							// ジャンプ速度
#define NORMAL_KNOCKBACK_SPEED 0.375f * 5.0f			// ノックバック速度
#define NORMAL_ATTACK_POWER 1							// 攻撃力
#define NORMAL_POWER 2									// ゴミを落とすパワー
#define NORMAL_RETURN_TIME 20.0f						// 撤退までの時間
#define NORMAL_COLLECTORS 1								// 回収員の人数
#define NORMAL_DEFAULT_BAG_NUM 5						// ゴミ袋の数の初期値
#define NORMAL_DEFAULT_GOLD_BAG_NUM 5					// ゴールドゴミ袋の数の初期値
// お仕置きのゴミ収集車のステータス
#define PUNISHER_MAX_HP 100								// 最大HP
#define PUNISHER_BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// 移動速度
#define PUNISHER_JUMP_SPEED 0.0f						// ジャンプ速度
#define PUNISHER_KNOCKBACK_SPEED 0.375f * 5.0f			// ノックバック速度
#define PUNISHER_ATTACK_POWER 1							// 攻撃力
#define PUNISHER_POWER 2								// ゴミを落とすパワー
#define PUNISHER_RETURN_TIME 1.0f						// 撤退までの時間
#define PUNISHER_COLLECTORS 1							// 回収員の人数
#define PUNISHER_DEFAULT_BAG_NUM 0						// ゴミ袋の数の初期値
#define PUNISHER_DEFAULT_GOLD_BAG_NUM 10					// ゴールドゴミ袋の数の初期値

CGarbageTruckStatus::CGarbageTruckStatus(bool punisher)
	: CVehicleStatus(NORMAL_MAX_HP, NORMAL_BASE_MOVE_SPEED, NORMAL_JUMP_SPEED,
		NORMAL_KNOCKBACK_SPEED, NORMAL_ATTACK_POWER, NORMAL_POWER)
	, mIsPunisher(punisher)
	, mReturnTime(NORMAL_RETURN_TIME)
	, mCollectorsNum(NORMAL_COLLECTORS)
	, mDefaultBagNum(NORMAL_DEFAULT_BAG_NUM)
	, mDefaultGoldBagNum(NORMAL_DEFAULT_GOLD_BAG_NUM)
{
	// お仕置き用なら
	if (punisher)
	{
		// お仕置き用のステータスに変更
		CVehicleStatus(PUNISHER_MAX_HP, PUNISHER_BASE_MOVE_SPEED, PUNISHER_JUMP_SPEED,
			PUNISHER_KNOCKBACK_SPEED, PUNISHER_ATTACK_POWER, PUNISHER_POWER);
		mReturnTime = PUNISHER_RETURN_TIME;
		mCollectorsNum = PUNISHER_COLLECTORS;
		mDefaultBagNum = PUNISHER_DEFAULT_BAG_NUM;
		mDefaultGoldBagNum = PUNISHER_DEFAULT_GOLD_BAG_NUM;
	}
}

CGarbageTruckStatus::~CGarbageTruckStatus()
{
}

// 撤退までの時間を取得
float CGarbageTruckStatus::GetReturnTime() const
{
	return mReturnTime;
}

// 撤退までの時間を初期値に設定
void CGarbageTruckStatus::SetReturnTime()
{
	// お仕置き用
	if (mIsPunisher)
	{
		mReturnTime = PUNISHER_RETURN_TIME;
	}
	// 通常用
	else
	{
		mReturnTime = NORMAL_RETURN_TIME;
	}
}

// 撤退までの時間が経過したかどうか
bool CGarbageTruckStatus::IsElapsedReturnTime() const
{
	// 0以下なら経過している
	return mReturnTime <= 0.0f;
}

// 撤退までの時間をカウントダウン
void CGarbageTruckStatus::CountReturnTime()
{
	mReturnTime -= Times::DeltaTime();
}

// 回収員の人数を取得する
int CGarbageTruckStatus::GetCollectorsNum() const
{
	return mCollectorsNum;
}

// 回収員の人数を初期値に設定する
void CGarbageTruckStatus::SetCollectorsNum()
{
	// お仕置き用
	if (mIsPunisher)
	{
		mCollectorsNum = PUNISHER_COLLECTORS;
	}
	// 通常用
	else
	{
		mCollectorsNum = NORMAL_COLLECTORS;
	}
}

// 回収員の人数を1減らす
void CGarbageTruckStatus::DecreaseCollectors()
{
	mCollectorsNum--;
}

// 初期から持っているゴミ袋の数を取得する
int CGarbageTruckStatus::GetDefaultBagNum() const
{
	return mDefaultBagNum;
}

// 初期から持っているゴールドゴミ袋の数を取得する
int CGarbageTruckStatus::GetDefaultGoldBagNum() const
{
	return mDefaultGoldBagNum;
}

// お仕置き用かどうかを取得
bool CGarbageTruckStatus::GetPunisher() const
{
	return mIsPunisher;
}


