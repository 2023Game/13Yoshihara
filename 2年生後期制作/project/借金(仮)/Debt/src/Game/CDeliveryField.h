#pragma once
#include "CFieldBase.h"

// スケールの倍率
#define SCALE_RATIO 1.5f

// 車道のZの範囲
#define ROAD_Z_AREA_P  170.0f	// プラス方向
#define ROAD_Z_AREA_M -300.0f	// マイナス方向

// 車道のX座標
#define ROAD_LEFT1_POSX		-90.0f
#define ROAD_LEFT2_POSX		-30.0f
#define ROAD_RIGHT1_POSX	 90.0f
#define ROAD_RIGHT2_POSX	 30.0f

// 強制移動速度
#define FORCE_MOVE_SPEED 7.5f * 60.0f	

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