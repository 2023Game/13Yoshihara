#pragma once
#include "CResultUIBase.h"

class CDeliveryGameResultUI : public CResultUIBase
{
public:
	// コンストラクタ
	CDeliveryGameResultUI();
	// デストラクタ
	~CDeliveryGameResultUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
};