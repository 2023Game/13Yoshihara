#pragma once
#include "CConnectObject.h"
#include "CBillBoardImage.h"

class CBillboardImage;

// 火とつなげると燃える草オブジェクト
class CGrass : public CConnectObject
{
public:
	// コンストラクタ
	CGrass();
	// デストラクタ
	~CGrass();

	// 繋がったときの処理
	void Connect(CConnectObject* other);

private:
	// コライダーを生成
	void CreateCol() override;

	// 草の画像
	CBillBoardImage* mpGrassImage;
};