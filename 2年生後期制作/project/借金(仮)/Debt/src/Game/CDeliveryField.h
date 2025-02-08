#pragma once
#include "CFieldBase.h"

#define ROAD_X_AREA 87.5f	// 車道のXの範囲

class CDeliveryField : public CFieldBase
{
public:
	// コンストラクタ
	CDeliveryField();
	// デストラクタ
	~CDeliveryField();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// フィールドのオブジェクト生成
	void CreateFieldObjects() override;
};