#pragma once
#include "CFire.h"
#include "CBillBoardImage.h"

class CBillboardImage;

// 火とつなげると燃える草オブジェクト
class CGrass : public CFire
{
public:
	// コンストラクタ
	CGrass(CVector fireOffsetPos = CVector::zero, float fireScale = FIRE_SCALE * 2.0f);
	// デストラクタ
	~CGrass();

	// 更新
	void Update() override;

private:
	// 草の画像
	CBillBoardImage* mpGrassImage;
};