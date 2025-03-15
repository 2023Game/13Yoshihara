#pragma once
#include "CDeliveryStatusBase.h"

// 配達の敵ステータスクラス
class CDeliveryEnemyStatus : public CDeliveryStatusBase
{
public:
	// コンストラクタ
	CDeliveryEnemyStatus();
	// デストラクタ
	~CDeliveryEnemyStatus();

	// 壊したら貰える得点を取得
	int GetScore() const;

private:
	// 壊したら貰える得点
	int mScore;
};