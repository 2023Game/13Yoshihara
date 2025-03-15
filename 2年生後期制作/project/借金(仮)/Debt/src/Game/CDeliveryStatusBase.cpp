#include "CDeliveryStatusBase.h"

// Hp一つ当たりの移動速度の減少値
#define SPEED_DOWN 7.5f

// コンストラクタ
CDeliveryStatusBase::CDeliveryStatusBase(int maxHp, float moveSpeed,
	float jumpSpeed, int attackPower, float throwSpeed)
	: CCharaStatusBase(maxHp, moveSpeed,
		jumpSpeed, attackPower)
	, mThrowSpeed(throwSpeed)
{
}

// デストラクタ
CDeliveryStatusBase::~CDeliveryStatusBase()
{
}

// 基礎移動速度を取得
float CDeliveryStatusBase::GetBaseMoveSpeed() const
{
	// 基礎移動速度を取得
	float baseMoveSpeed = CCharaStatusBase::GetBaseMoveSpeed();
	// 一つ当たりの減少値×HP、を引いて移動速度を計算
	float moveSpeed = baseMoveSpeed - SPEED_DOWN * GetHp();
	// 0以下なら0にする
	if (moveSpeed <= 0.0f) moveSpeed = 0.0f;
	return moveSpeed;
}

// 投げる速度を取得
float CDeliveryStatusBase::GetThrowSpeed() const
{
	return mThrowSpeed;
}
