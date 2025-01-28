#pragma once
#include "CFieldBase.h"

class CVehicleBase;

#define ROAD_X_AREA 85.0f	// 車道のXの範囲
#define ROAD_Z_AREA 90.0f	// 車道のZの範囲

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

	// 地面のコライダ―取得
	CCollider* GetGroundCol();
	// 壁のコライダ―取得
	CCollider* GetWallCol();
	// オブジェクトのコライダ―取得
	CCollider* GetObjCol();

private:
	// フィールドのオブジェクト生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};