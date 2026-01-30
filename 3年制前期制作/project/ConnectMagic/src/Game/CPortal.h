#pragma once
#include "CImage3D.h"

// ポータル
class CPortal : public CImage3D
{
public:
	// コンストラクタ
	CPortal();
	// デストラクタ
	~CPortal();

private:
	// コライダーを生成
	void CreateCol() override;
	// 衝突処理
	void OnSensorEnter(const CollisionData& data) override;
};