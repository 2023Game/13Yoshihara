#pragma once
#include "CCharaStatusBase.h"

// 配達のプレイヤーステータスクラス
class CDeliveryPlayerStatus : public CCharaStatusBase
{
public:
	// コンストラクタ
	CDeliveryPlayerStatus();
	// デストラクタ
	~CDeliveryPlayerStatus();

	// 基礎移動速度を取得
	float GetBaseMoveSpeed() const override;
	// 投げる速度を取得
	float GetThrowSpeed() const;
private:
	float mThrowSpeed;
};