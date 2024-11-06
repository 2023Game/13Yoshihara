#pragma once
#include "CJobStatusBase.h"
class CTrashStatus;
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
	bool GetUnlock(EJobType jobType);
	/// <summary>
	/// 各仕事のアンロック状態を設定する
	/// </summary>
	/// <param name="jobType">仕事の種類</param>
	/// <param name="unlock">アンロック状態(trueでアンロック)</param>
	void SetUnlock(EJobType jobType, bool unlock);

	// 各仕事の強化倍率を取得する
	float GetUpgrade(EJobType jobType);
	/// <summary>
	/// 各仕事の強化倍率を設定する
	/// </summary>
	/// <param name="jobType">仕事の種類</param>
	/// <param name="upgrade">強化倍率の増加量</param>
	void SetUpgrade(EJobType jobType, float upgrade);
private:
	// 仕事ステータスマネージャのインスタンス
	static CJobStatusManager* mpInstance;
	CTrashStatus* mTrashStatus;
	CDeliveryStatus* mDeliveryStatus;
};