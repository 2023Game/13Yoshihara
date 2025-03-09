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

	// スコアを計算して取得する
	int GetScore() const override;

	// 配達1個当たりの得点を取得
	int GetDeliveryItemScore() const;
	// 敵を1体壊すと貰える得点を取得
	int GetDestroyEnemyScore() const;

private:
	// 配達1個当たりの得点
	int mDeliveryItemScore;
	// 敵を1体壊すと貰える得点
	int mDestroyEnemyScore;
};