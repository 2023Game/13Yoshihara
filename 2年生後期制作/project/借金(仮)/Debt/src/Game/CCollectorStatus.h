#pragma once
#include "CTrashStatusBase.h"

/*
ゴミ拾いゲームの回収員のステータス
ゴミ拾いゲームのステータス基底クラスを継承
*/
class CCollectorStatus : public CTrashStatusBase
{
public:
	// コンストラクタ
	CCollectorStatus(bool punisher);
	// デストラクタ
	~CCollectorStatus();

private:
	// お仕置き用かどうか
	bool mIsPunisher;
};