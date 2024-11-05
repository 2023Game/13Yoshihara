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
	// 各仕事の解除状態を取得する

	// 各仕事の解除状態を設定する

	// 各仕事の強化値を取得する

	// 各仕事の強化値を設定する
private:
	// 仕事ステータスマネージャのインスタンス
	static CJobStatusManager* mpInstance;
	CTrashStatus* mTrashStatus;
	CDeliveryStatus* mDeliveryStatus;
};