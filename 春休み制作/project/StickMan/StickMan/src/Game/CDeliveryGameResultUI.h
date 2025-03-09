#pragma once
#include "CResultUIBase.h"

// 配達のリザルトUIクラス
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
	// 配達物の画像
	CImage* mpDeliveryItemImg;
	// トラックの画像
	CImage* mpDeliveryTruckImg;
};