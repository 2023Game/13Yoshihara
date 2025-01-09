#pragma once
// ゴミ拾いゲームのステータス基底クラスをインクルード
#include "CTrashStatusBase.h"

/*
ゴミ拾いゲームのプレイヤーステータス
ゴミ拾いゲームのステータス基底クラスを継承
*/
class CTrashPlayerStatus : public CTrashStatusBase
{
public:
	// コンストラクタ
	CTrashPlayerStatus();
	// デストラクタ
	~CTrashPlayerStatus();

	// クリティカル確率を取得
	int GetCriticalChance();

private:
	// クリティカル確率
	int mCriticalChance;
};