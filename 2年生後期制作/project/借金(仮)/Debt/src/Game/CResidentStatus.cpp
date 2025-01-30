#include "CResidentStatus.h"
#include "Maths.h"

#define MAX_HP 0						// 最大HP
#define BASE_MOVE_SPEED 0.375f * 20.0f	// 移動速度
#define JUMP_SPEED 0.0f					// ジャンプ速度
#define ATTACK_POWER 0					// 攻撃力
#define THROW_SPEED 2.2f				// 投げる速度
#define THROW_TIME 1.0f					// ゴミを投げるまでの時間
#define MAX_THROW_NUM 3					// 投げれるゴミの最大数
#define GOLD_CHANCE 10					// ゴールド袋が出る確率

// ゴミを投げる速度を計算するときランダムで掛ける最大値
#define THROW_SPEED_RANDOM_MAX 1.5f
// ゴミを投げるまでの時間を計算するときランダムで掛ける最大数
#define THROW_TIME_RANDOM_MAX 5

// コンストラクタ
CResidentStatus::CResidentStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED, ATTACK_POWER)
	, mThrowSpeed(THROW_SPEED)
	, mThrowTime(THROW_TIME)
	, mMaxThrowNum(MAX_THROW_NUM)
	, mGoldChance(GOLD_CHANCE)
{
	// ランダム数値を掛けた値を設定
	SetThrowTime();
}

// デストラクタ
CResidentStatus::~CResidentStatus()
{
}

// ゴミ袋を投げる速度を設定する
void CResidentStatus::SetThrowSpeed()
{
	// 等倍から最大値までのランダムな数値を掛ける
	int random = Math::Rand(1.0f, THROW_SPEED_RANDOM_MAX);
	mThrowSpeed = THROW_SPEED * random;
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
	// 等倍から最大数までのランダムな数値を掛ける
	int random = Math::Rand(1, THROW_TIME_RANDOM_MAX);
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

// ゴミ袋の最大数を設定
void CResidentStatus::SetMaxThrowNum(int maxThrowNum)
{
	mMaxThrowNum = maxThrowNum;
}

// ゴミ袋の最大数を取得
int CResidentStatus::GetMaxThrowNum() const
{
	return mMaxThrowNum;
}

// ゴールド確率を設定
void CResidentStatus::SetGoldChance(int goldChance)
{
	mGoldChance = goldChance;
}

// ゴールド確率を取得
int CResidentStatus::GetGoldChance() const
{
	return mGoldChance;
}
