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

	// スコアを計算して取得する
	int GetScore() const override;

	// ゴミ袋一つの得点を取得
	int GetTrashBagScore() const;
	// ゴールドのゴミ袋一つの得点を取得
	int GetGoldTrashBagScore() const;

private:
	// ゴミ袋一つの得点
	int mTrashBagScore;
	// ゴールドのゴミ袋一つの得点
	int mGoldTrashBagScore;
};