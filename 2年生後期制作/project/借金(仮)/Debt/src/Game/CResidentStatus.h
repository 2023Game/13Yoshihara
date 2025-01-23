#pragma once
#include "CCharaStatusBase.h"

class CResidentStatus : public CCharaStatusBase
{
public:
	// コンストラクタ
	CResidentStatus();
	// デストラクタ
	~CResidentStatus();

	// ゴミ袋を投げる速度を初期値にランダム数値を掛けた値に設定する
	void SetThrowSpeed();
	// ゴミ袋を投げる速度を取得する
	float GetThrowSpeed() const;

	// ゴミを投げるまでの時間を取得
	float GetThrowTime() const;
	// ゴミを投げるまでの時間を初期値にランダム数値を掛けた数に設定
	void SetThrowTime();
	// ゴミを投げるまでの時間が経過したかどうか
	bool IsElapsedThrowTime() const;
	// ゴミを投げるまでの時間をカウントダウン
	void CountThrowTime();

	// ゴミ袋の最大数を設定
	void SetMaxThrowNum(int maxThrowNum);
	// ゴミ袋の最大数を取得
	int GetMaxThrowNum() const;

	// ゴールド確率を設定
	void SetGoldChance(int goldChance);
	// ゴールド確率を取得
	int GetGoldChance() const;
private:
	float mThrowSpeed;	// ゴミ袋を投げる速度
	float mThrowTime;	// ゴミ袋を投げるまでの時間
	int mMaxThrowNum;	// フィールド上に同時に存在させれるゴミ袋の数
	// ゴールド確率
	int mGoldChance;
};