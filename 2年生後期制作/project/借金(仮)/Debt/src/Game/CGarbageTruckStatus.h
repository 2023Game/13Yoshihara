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

	// ゴミ袋の数の初期値を取得する
	int GetDefaultBagNum() const;
	// ゴールドゴミ袋の数の初期値を取得する
	int GetDefaultGoldBagNum() const;

	// お仕置き用かどうかを取得
	bool GetPunisher() const;
private:
	float mReturnTime;		// 撤退までの時間
	int mCollectorsNum;		// 回収員の人数
	bool mIsPunisher;		// お仕置き用かどうか
	int mDefaultBagNum;		// ゴミ袋の数の初期値
	int mDefaultGoldBagNum;	// ゴールドゴミ袋の数の初期値
};