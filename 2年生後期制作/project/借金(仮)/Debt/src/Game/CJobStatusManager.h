#pragma once
#include "CJobStatusBase.h"
class CTrashJobStatus;
class CDeliveryJobStatus;

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

	// コンストラクタ
	CJobStatusManager();
	// デストラクタ
	~CJobStatusManager();

	// 各仕事のアンロック状態を取得する
	bool GetUnlock(EJobType jobType) const;
	// 各仕事のアンロック状態を設定する
	void SetUnlock(EJobType jobType, bool unlock);

	// 各仕事のアンロック金額を取得
	int GetUnlockMoney(EJobType jobType) const;
	// 各仕事のアンロック金額を設定
	void SetUnlockMoney(EJobType jobType, int unlockMoney);

	// 各仕事のボーナスの強化値を取得する
	float GetBonus(EJobType jobType) const;
	// 各仕事のボーナスの強化値を設定する
	void SetBonus(EJobType jobType, float bonus);

	// 各仕事のHPの強化値を取得
	int GetHp(EJobType jobType) const;
	// 各仕事のHPの強化値を設定
	void SetHp(EJobType jobType, int hp);

	// 各仕事の移動速度の強化値を取得
	float GetSpeed(EJobType jobType) const;
	// 各仕事の移動速度の強化値を設定
	void SetSpeed(EJobType jobType, float speed);

	// 各仕事の攻撃力の強化値を取得
	int GetPower(EJobType jobType) const;
	// 各仕事の攻撃力の強化値を設定
	void SetPower(EJobType jobType, int power);

	//// 各ジョブのポインタを取得
	//CTrashJobStatus* GetTrashStatus();
	//CDeliveryJobStatus* GetDeliveryStatus();

	// 選択されている仕事を取得
	EJobType GetSelectJob() const;
	// 選択されている仕事を設定
	void SetSelectJob(EJobType jobType);

	// 全ての前の日のステータスを今のステータスに設定
	void SetPreJobStatus();
	// 前日にロールバックする
	void Rollback();

	// 初めてのプレイかを取得
	bool GetFirstPlay(EJobType jobType);
	// 初めてのプレイか設定
	void SetFirstPlay(EJobType jobType, bool isFirst);

private:
	// 選択されている仕事
	EJobType mSelectJob;
	// 前の日に選択されていた仕事
	EJobType mPreSelectJob;
	// 仕事ステータスマネージャのインスタンス
	static CJobStatusManager* mpInstance;
	// 各ジョブのステータス
	CTrashJobStatus* mpTrashStatus;
	CDeliveryJobStatus* mpDeliveryStatus;
};