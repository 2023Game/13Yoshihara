#pragma once
#include "CJobStatusBase.h"
class CTrashJobStatus;
class CDeliveryStatus;

class CJobStatusManager
{
public:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static CJobStatusManager* Instance();
	// インスタンスを破棄
	void ClearInstance();

	CJobStatusManager();
	~CJobStatusManager();

	void Update();

	// TODO
	// 各仕事のアンロック状態を取得する
	bool GetUnlock(EJobType jobType) const;
	/// <summary>
	/// 各仕事のアンロック状態を設定する
	/// </summary>
	/// <param name="jobType">仕事の種類</param>
	/// <param name="unlock">アンロック状態(trueでアンロック)</param>
	void SetUnlock(EJobType jobType, bool unlock);

	// 各仕事の強化倍率を取得する
	float GetUpgrade(EJobType jobType) const;
	/// <summary>
	/// 各仕事の強化倍率を設定する
	/// </summary>
	/// <param name="jobType">仕事の種類</param>
	/// <param name="upgrade">強化倍率の増加量</param>
	void SetUpgrade(EJobType jobType, float upgrade);

	// 各ジョブのポインタを取得
	CTrashJobStatus* GetTrashStatus();
	CDeliveryStatus* GetDeliveryStatus();

	// 選択されている仕事を取得
	EJobType GetSelectJob() const;
	// 選択されている仕事を設定
	void SetSelectJob(EJobType jobType);
private:
	// 選択されている仕事
	EJobType mSelectJob;
	// 仕事ステータスマネージャのインスタンス
	static CJobStatusManager* mpInstance;
	// 各ジョブのステータス
	CTrashJobStatus* mTrashStatus;
	CDeliveryStatus* mDeliveryStatus;
};