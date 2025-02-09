#include "CDeliveryItemStatus.h"

#define SCORE 100	// 一つ当たりの得点
#define DAMAGE 1	// 与えるダメージ

// コンストラクタ
CDeliveryItemStatus::CDeliveryItemStatus()
	: mScore(SCORE)
	, mDamage(DAMAGE)
{
}

// デストラクタ
CDeliveryItemStatus::~CDeliveryItemStatus()
{
}

// 一つ当たりの得点を取得
int CDeliveryItemStatus::GetScore() const
{
	return mScore;
}

// 与えるダメージを取得
int CDeliveryItemStatus::GetDamage() const
{
	return mDamage;
}
