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
	~CJobStatusBase();

	void Update();

	// アンロック状態を取得
	bool GetUnlock();
	// アンロック状態を設定
	void SetUnlock(bool unlock);

	// 強化倍率を取得
	float GetUpgrade();
	/// <summary>
	/// 強化倍率を設定
	/// </summary>
	/// <param name="upgrade">強化倍率の増加量</param>
	void SetUpgrade(float upgrade);

	// 仕事の種類を取得
	EJobType GetJobType();

private:
	// 仕事ステータスマネージャのインスタンス
	CJobStatusBase* mpInstance;

	// 仕事の種類
	EJobType mJobType;
	
	// アンロック状態（初期値falseで未解除）
	bool mUnlock;
	
	// 強化倍率（初期値1.0f）
	float mUpgrade;
};