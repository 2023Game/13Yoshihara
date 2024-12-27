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
	CTrashEnemyStatus();
	// デストラクタ
	~CTrashEnemyStatus();
};