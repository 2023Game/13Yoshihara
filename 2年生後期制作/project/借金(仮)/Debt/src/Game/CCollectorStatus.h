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

	// お仕置き用かどうかを取得する
	bool GetPunisher() const;
private:
	// お仕置き用かどうか
	bool mIsPunisher;
};