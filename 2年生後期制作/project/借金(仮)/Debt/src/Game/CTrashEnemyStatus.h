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
private:
	// クリティカル確率
	int mCriticalChance;
	// お仕置き用かどうか
	bool mIsPunisher;
};