#pragma once
// ゴミ拾いゲームのステータス基底クラスをインクルード
#include "CTrashStatusBase.h"

/*
ゴミ拾いゲームの敵ステータス
ゴミ拾いゲームのステータス基底クラスを継承
*/
class CTrashEnemyStatus : public CTrashStatusBase
{
public:
	// コンストラクタ
	CTrashEnemyStatus(bool punisher);
	// デストラクタ
	~CTrashEnemyStatus();

	// クリティカル確率を取得
	int GetCriticalChance();
	// クリティカル確率を設定
	void SetCriticalChance(int criticalChance);

	// ゴミ袋の数の初期値を取得する
	int GetDefaultBagNum() const;
	// ゴールドゴミ袋の数の初期値を取得する
	int GetDefaultGoldBagNum() const;

private:
	int mCriticalChance;	// クリティカル確率
	bool mIsPunisher;		// お仕置き用かどうか
	int mDefaultBagNum;		// ゴミ袋の数の初期値
	int mDefaultGoldBagNum;	// ゴールドゴミ袋の数の初期値
};