#include "CResidentStatus.h"
#include "Maths.h"

#define MAX_HP 0						// 最大HP
#define BASE_MOVE_SPEED 0.375f * 20.0f	// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define ATTACK_POWER 0					// 攻撃力
#define THROW_SPEED 1.0f				// 投げる速度
#define THROW_TIME 5.0f					// ゴミを投げるまでの時間

// コンストラクタ
CResidentStatus::CResidentStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED, ATTACK_POWER)
	, mThrowSpeed(THROW_SPEED)
	, mThrowTime(THROW_TIME)
{
	// ランダム数値を掛けた値を設定
	SetThrowTime();
}

// デストラクタ
CResidentStatus::~CResidentStatus()
{
}

// ゴミ袋を投げる速度を設定する
void CResidentStatus::SetThrowSpeed(float throwSpeed)
{
	mThrowSpeed = throwSpeed;
}

// ゴミ袋を投げる速度を取得する
float CResidentStatus::GetThrowSpeed() const
{
	return mThrowSpeed;
}

// ゴミを投げるまでの時間を取得
float CResidentStatus::GetThrowTime() const
{
	return mThrowTime;
}

// ゴミを投げるまでの時間を初期値にランダム数値を掛けた数に設定
void CResidentStatus::SetThrowTime()
{
	// 等倍から1.5倍までのランダムな数値を掛ける
	float random = Math::Rand(1.0f, 2.0f);
	mThrowTime = THROW_TIME * random;
}

// ゴミを投げるまでの時間が経過したかどうか
bool CResidentStatus::IsElapsedThrowTime() const
{
	// 0以下なら経過している
	return mThrowTime <= 0.0f;
}

// ゴミを投げるまでの時間をカウントダウン
void CResidentStatus::CountThrowTime()
{
	mThrowTime -= Times::DeltaTime();
}
