#pragma once

class CTextUI2D;
class CTrashBagStatus;

// ゴミ拾いのスコア表示UI
class CTrashScoreUI
{
public:
	// スコア表示UIのインスタンスを取得
	static CTrashScoreUI* Instance();
	// コンストラクタ
	CTrashScoreUI();
	// デストラクタ
	~CTrashScoreUI();

	// 更新
	void Update();

	// スコアを取得する
	int GetScore() const;

private:
	// スコア表示UIのインスタンス
	static CTrashScoreUI* spInstance;
	// スコア
	int mScore;

	// ゴミ袋一つの得点
	int mTrashBagScore;
	// ゴールドのゴミ袋一つの得点
	int mGoldTrashBagScore;
	// テキストUI
	CTextUI2D* mpScoreUI;
};