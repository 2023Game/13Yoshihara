#pragma once
#include "CCharaStatusBase.h"

class CResidentStatus : public CCharaStatusBase
{
public:
	// コンストラクタ
	CResidentStatus();
	// デストラクタ
	~CResidentStatus();

	// ゴミ袋を投げる速度を設定する
	void SetThrowSpeed(float throwSpeed);
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
private:
	float mThrowSpeed;	// ゴミ袋を投げる速度
	float mThrowTime;	// ゴミ袋を投げるまでの時間
};