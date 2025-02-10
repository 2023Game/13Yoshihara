#include "CTrashBagStatus.h"

#define DELETE_TIME 5.0f	// 消滅までの時間

#define NORMAL_POINT 100	// 通常のポイント
#define GOLD_POINT 1000		// ゴールドのポイント

// コンストラクタ
CTrashBagStatus::CTrashBagStatus(bool gold)
	: mIsGold(gold)
	, mDeleteTime(DELETE_TIME)
{
}

// デストラクタ
CTrashBagStatus::~CTrashBagStatus()
{
}

// ゴールドかどうかを取得する
bool CTrashBagStatus::GetIsGold() const
{
	return mIsGold;
}

// ゴールドかどうかを設定する
void CTrashBagStatus::SetIsGold(bool isGold)
{
	mIsGold = isGold;
}

// 消滅までの時間を取得する
float CTrashBagStatus::GetDeleteTime() const
{
	return mDeleteTime;
}

// 消滅までの時間を初期値に設定する
void CTrashBagStatus::SetDeleteTime()
{
	mDeleteTime = DELETE_TIME;
}

// 消滅までの時間が経過したかどうか
bool CTrashBagStatus::IsElapsedDeleteTime() const
{
	// 消滅までの時間が0以下なら経過した
	return mDeleteTime <= 0.0f;
}

// 消滅までの時間をカウントダウン
void CTrashBagStatus::CountDeleteTime()
{
	mDeleteTime -= Times::DeltaTime();
}

// 得点を取得する
int CTrashBagStatus::GetPoint(bool gold) const
{
	// ゴールドなら
	if (gold)
	{
		// ゴールドのポイントを返す
		return GOLD_POINT;
	}
	// 通常なら
	else
	{
		// 通常のポイントを返す
		return NORMAL_POINT;
	}
}
