#pragma once
#include "JobType.h"

class CJobStatusBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="jobType">仕事の種類</param>
	CJobStatusBase(EJobType jobType);
	// デストラクタ
	~CJobStatusBase();

	// 更新
	void Update();

	// アンロック状態を取得
	bool GetUnlock() const;
	// アンロック状態を設定
	void SetUnlock(bool unlock);

	// アンロックの金額を取得
	int GetUnlockMoney() const;
	// アンロックの金額を設定
	void SetUnlockMoney(int unlockMoney);

	// ボーナスの強化値を取得
	float GetBonus() const;
	// ボーナスの強化値を設定
	void SetBonus(float bonus);

	// HPの強化値を取得
	int GetHp() const;
	// HPの強化値を設定
	void SetHp(int hp);

	// 移動速度の強化値を取得
	float GetSpeed() const;
	// 移動速度の強化値を設定
	void SetSpeed(float speed);

	// 攻撃力の強化値を取得
	int GetPower() const;
	// 攻撃力の強化値を設定
	void SetPower(int power);

	// 仕事の種類を取得
	EJobType GetJobType();

private:
	// 仕事の種類
	EJobType mJobType;
	
	// アンロック状態（初期値falseで未解除）
	bool mUnlock;
	// アンロック金額
	int mUnlockMoney;

	// ボーナスの強化値（初期値0.0f）
	float mBonusUpgrade;
	// HPの強化値（初期値0）
	int mHpUpgrade;
	// 移動速度の強化値（初期値0.0f）
	float mSpeedUpgrade;
	// 攻撃力の強化値（初期値0）
	int mPowerUpgrade;
};