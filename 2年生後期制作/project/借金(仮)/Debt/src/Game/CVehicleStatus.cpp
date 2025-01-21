#include "CVehicleStatus.h"

// 使わないベースステータスは値が0
#define MAX_HP 0								// 最大HP
#define BASE_MOVE_SPEED 0.375f * 5.0f * 60.0f	// 移動速度
#define JUMP_SPEED 0.0f							// ジャンプ速度
#define KNOCKBACK_SPEED 0.375f * 5.0f			// ノックバック速度
#define ATTACK_POWER 1							// 攻撃力
#define POWER 1									// ゴミを落とすパワー

#define DEATH_WAIT_TIME 2.0f	// 消滅までの時間

// コンストラクタ
CVehicleStatus::CVehicleStatus()
	: CTrashStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED,
		KNOCKBACK_SPEED, ATTACK_POWER, POWER)
	, mDeleteTime(DEATH_WAIT_TIME)
{
}

/*
コンストラクタ
このクラスを継承したキャラのステータスの設定用
*/
CVehicleStatus::CVehicleStatus(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockback, int attackPower, int power)
	: CTrashStatusBase(maxHp, baseMoveSpeed, jumpSpeed,
		knockback, attackPower, power)
	, mDeleteTime(DEATH_WAIT_TIME)
{
}

CVehicleStatus::~CVehicleStatus()
{
}

// 消滅までの時間を取得する
float CVehicleStatus::GetDeleteTime() const
{
	return mDeleteTime;
}

// 消滅までの時間を初期値に設定する
void CVehicleStatus::SetDeleteTime()
{
	mDeleteTime = DEATH_WAIT_TIME;
}

// 消滅までの時間が経過したかどうか
bool CVehicleStatus::IsElapsedDeleteTime() const
{
	// 消滅までの時間が0より大きいなら経過していない
	if (mDeleteTime > 0.0f)
	{
		return false;
	}
	// 0以下なら経過した
	else
	{
		return true;
	}
}

// 消滅までの時間をカウントダウン
void CVehicleStatus::CountDeleteTime()
{
	// 0より大きいなら減算
	if (mDeleteTime > 0.0f)
	{
		mDeleteTime -= Times::DeltaTime();
	}
}


