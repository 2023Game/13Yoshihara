#pragma once
#include "CCharaStatusBase.h"

// 配達の敵ステータスクラス
class CDeliveryEnemyStatus : public CCharaStatusBase
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