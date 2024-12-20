#pragma once
#include "CCarStatus.h"

// ゴミ収集車のステータス
// 車のステータスに追加でこのステータスを付与
class CGarbageTruckStatus : public CCarStatus
{
public:
	CGarbageTruckStatus();
	~CGarbageTruckStatus();

	// 撤退までの時間を取得
	float GetBackTime();
	// 撤退までの時間を設定
	void SetBackTime(float backTime);
	// 撤退までの時間をカウント
	void CountBackTime();

	// 回収範囲の半径を取得する
	float GetCollectRadius();

	// 回収員の人数を取得する
	int GetCollectors();
	// 回収員の人数を設定する
	void SetCollectors(int collectors);
private:
	float mBackTime;		// 撤退までの時間
	float mCollectRadius;	// 回収範囲の半径
	int mCollectors;		// 回収員の人数
};