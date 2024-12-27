#pragma once
#include "CVehicleStatus.h"

// ゴミ収集車のステータス
class CGarbageTruckStatus : public CVehicleStatus
{
public:
	// コンストラクタ
	CGarbageTruckStatus();
	// デストラクタ
	~CGarbageTruckStatus();

	// 撤退までの時間を取得
	float GetBackTime() const;
	// 撤退までの時間を初期値に設定
	void SetBackTime();
	// 撤退までの時間が経過したかどうか
	bool IsElapsedBackTime() const;
	// 撤退までの時間をカウントダウン
	void CountBackTime();

	// 回収範囲の半径を取得する
	float GetCollectRadius() const;

	// 回収員の人数を取得する
	int GetCollectors() const;
	// 回収員の人数を初期値に設定する
	void SetCollectors();
	// 回収員の人数を1減らす
	void DecreaseCollectors();
private:
	float mBackTime;		// 撤退までの時間
	float mCollectRadius;	// 回収範囲の半径
	int mCollectors;		// 回収員の人数
};