#pragma once

class CTextUI2D;
class CTrashBagStatus;

// スコア表示UIの基底クラス
class CScoreUIBase
{
public:
	// スコア表示UIのインスタンスを取得
	static CScoreUIBase* Instance();
	// コンストラクタ
	CScoreUIBase();
	// デストラクタ
	~CScoreUIBase();

	// 更新
	void Update();

	// スコアを計算して取得する
	virtual int GetScore() const = 0;

protected:
	// スコア表示UIのインスタンス
	static CScoreUIBase* spInstance;

	// テキストUI
	CTextUI2D* mpScoreUI;
};