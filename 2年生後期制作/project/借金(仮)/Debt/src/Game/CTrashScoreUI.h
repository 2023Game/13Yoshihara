#pragma once

class CTextUI2D;
class CTrashBagStatus;

// ゴミ拾いのスコア表示UI
class CTrashScoreUI
{
public:
	// コンストラクタ
	CTrashScoreUI();
	// デストラクタ
	~CTrashScoreUI();

	// 更新
	void Update();

	// スコアを取得する
	int GetScore() const;

private:
	// スコア
	int mScore;

	// ゴミ袋ごとの得点取得用
	CTrashBagStatus* mpTrashBagStatus;
	// テキストUI
	CTextUI2D* mpScoreUI;
};