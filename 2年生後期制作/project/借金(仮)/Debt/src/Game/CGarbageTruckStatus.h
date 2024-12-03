#pragma once
#include "CTrashStatusBase.h"

class CGarbageTruckStatus : public CTrashStatusBase
{
public:
	CGarbageTruckStatus();
	~CGarbageTruckStatus();

	// 出現までの時間を取得する
	float GetPopTime();
	// 出現までの時間を設定する
	void SetPopTime(float popTime);

	// 消滅までの時間を取得する
	float GetDeleteTime();
	// 消滅までの時間を設定する
	void SetDeleteTime(float deleteTime);

	// 撤退までの時間を取得する
	float GetBackTime();
	// 撤退までの時間を設定する
	void SetBackTime(float backTime);

	// 回収範囲の半径を取得する
	float GetCollectRadius();

	// 回収員の人数を取得する
	int GetCollectors();
	// 回収員の人数を設定する
	void SetCollectors(int collectors);
protected:
	float mPopTime;			// 出現までの時間
	float mDeleteTime;		// 消滅までの時間
	float mBackTime;		// 撤退までの時間
	float mCollectRadius;	// 回収範囲の半径
	int mCollectors;		// 回収員の人数
};