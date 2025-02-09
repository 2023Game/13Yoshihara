#pragma once
#include "CScoreUIBase.h"

// ゴミ拾いのスコア表示UI
class CTrashScoreUI : public CScoreUIBase
{
public:
	// コンストラクタ
	CTrashScoreUI();
	// デストラクタ
	~CTrashScoreUI();

	// 更新
	void Update();

	// スコアを取得する
	int GetScore() const override;

private:
	// ゴミ袋一つの得点
	int mTrashBagScore;
	// ゴールドのゴミ袋一つの得点
	int mGoldTrashBagScore;
};