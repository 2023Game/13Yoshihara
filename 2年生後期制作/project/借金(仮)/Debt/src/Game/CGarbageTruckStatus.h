#pragma once
#include "CVehicleStatus.h"

// ゴミ収集車のステータス
class CGarbageTruckStatus : public CVehicleStatus
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="punisher">trueなら、お仕置き用</param>
	CGarbageTruckStatus(bool punisher);
	// デストラクタ
	~CGarbageTruckStatus();

	// 撤退までの時間を取得
	float GetReturnTime() const;
	// 撤退までの時間を初期値に設定
	void SetReturnTime();
	// 撤退までの時間が経過したかどうか
	bool IsElapsedReturnTime() const;
	// 撤退までの時間をカウントダウン
	void CountReturnTime();


	// 回収員の人数を取得する
	int GetCollectorsNum() const;
	// 回収員の人数を初期値に設定する
	void SetCollectorsNum();
	// 回収員の人数を1減らす
	void DecreaseCollectors();
private:
	float mReturnTime;	// 撤退までの時間
	int mCollectorsNum;		// 回収員の人数
	bool mIsPunisher;		// お仕置き用かどうか
};