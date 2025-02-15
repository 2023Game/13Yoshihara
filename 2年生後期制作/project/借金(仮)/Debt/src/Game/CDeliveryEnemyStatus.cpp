#include "CDeliveryEnemyStatus.h"

#define MAX_HP 10						// 最大HP
#define BASE_MOVE_SPEED 7.5f * 60.0f	// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define ATTACK_POWER 1					// 攻撃力
#define SCORE 5000						// 得点
#define THROW_SPEED	7.5f * 60.0f		// 投げる速度

// Hp一つ当たりの移動速度の減少値
#define SPEED_DOWN 7.5f

// コンストラクタ
CDeliveryEnemyStatus::CDeliveryEnemyStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, ATTACK_POWER)
	, mScore(SCORE)
	, mThrowSpeed(THROW_SPEED)
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

// 基礎移動速度を取得
float CDeliveryEnemyStatus::GetBaseMoveSpeed() const
{
	// 基礎移動速度を取得
	float baseMoveSpeed = CCharaStatusBase::GetBaseMoveSpeed();
	// 一つ当たりの減少値×HP、を引いて移動速度を計算
	float moveSpeed = baseMoveSpeed - SPEED_DOWN * GetHp();
	// 0以下なら0にする
	if (moveSpeed <= 0.0f) moveSpeed = 0.0f;
	return moveSpeed;
}

// 投げる速度を取得
float CDeliveryEnemyStatus::GetThrowSpeed() const
{
	return mThrowSpeed;
}
