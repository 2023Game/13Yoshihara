#pragma once
#include "CTrashStatusBase.h"

class CCarStatus : public CTrashStatusBase
{
public:
	CCarStatus();
	~CCarStatus();

	// 消滅までの時間を取得する
	float GetDeleteTime();
	// 消滅までの時間を設定する
	void SetDeleteTime(float deleteTime);
	// 消滅までの時間が経過したかどうか
	bool IsElapsedDeleteTime();
	// 消滅までの時間をカウント
	void CountDeleteTime();
private:
	float mDeleteTime;	// 消滅までの時間
};