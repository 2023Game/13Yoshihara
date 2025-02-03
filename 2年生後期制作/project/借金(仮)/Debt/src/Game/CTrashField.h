#pragma once
#include "CFieldBase.h"

class CVehicleBase;

#define ROAD_X_AREA 90.0f	// 車道のXの範囲
#define ROAD_Z_AREA 200.0f	// 車道のZの範囲

class CTrashField : public CFieldBase
{
public:
	// コンストラクタ
	CTrashField();
	// デストラクタ
	~CTrashField();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// フィールドのオブジェクト生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};