#pragma once
#include "CCharaStatusBase.h"

// 配達の敵ステータスクラス
class CDeliveryEnemyStatus : public CCharaStatusBase
{
public:
	// コンストラクタ
	CDeliveryEnemyStatus();
	// デストラクタ
	~CDeliveryEnemyStatus();

	// 壊したら貰える得点を取得
	int GetScore() const;

	// 基礎移動速度を取得
	float GetBaseMoveSpeed() const override;
	// 投げる速度を取得
	float GetThrowSpeed() const;
private:
	// 壊したら貰える得点
	int mScore;
	// 投げる速度
	float mThrowSpeed;
};