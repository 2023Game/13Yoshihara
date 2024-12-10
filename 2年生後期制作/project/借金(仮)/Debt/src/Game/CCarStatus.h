#pragma once
#include "CTrashStatusBase.h"

class CCarStatus : public CTrashStatusBase
{
public:
	CCarStatus();
	~CCarStatus();

	// 消滅までの時間を取得する
	float GetDeleteTime() const;
	// 消滅までの時間を設定する
	void SetDeleteTime(float deleteTime);
	// 消滅までの時間が経過したかどうか
	bool IsElapsedDeleteTime() const;
	// 消滅までの時間をカウント
	void CountDeleteTime();
private:
	float mDeleteTime;	// 消滅までの時間
};