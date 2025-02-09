#pragma once
#include "CScoreUIBase.h"

class CDeliveryScoreUI : public CScoreUIBase
{
public:
	// コンストラクタ
	CDeliveryScoreUI();
	// デストラクタ
	~CDeliveryScoreUI();

	// 更新
	void Update();

	// スコアを取得する
	int GetScore() const override;

private:
	// 配達1個当たりの得点
	int mDeliveryItemScore;
	// 敵を1体壊すと貰える得点
	int mDestroyEnemyScore;
};