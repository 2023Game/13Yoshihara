#pragma once
#include "CCharaStatusBase.h"

// 配達の基底ステータスクラス
class CDeliveryStatusBase : public CCharaStatusBase
{
public:
	// コンストラクタ
	CDeliveryStatusBase(int maxHp, float moveSpeed,
		float jumpSpeed, int attackPower, float throwSpeed);
	// デストラクタ
	~CDeliveryStatusBase();

	// 基礎移動速度を取得
	float GetBaseMoveSpeed() const override;
	// 投げる速度を取得
	float GetThrowSpeed() const;

private:
	// 投げる速度
	float mThrowSpeed;
};