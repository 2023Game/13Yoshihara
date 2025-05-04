#pragma once
#include "CBillBoardImage.h"

// 接続部分のエフェクト
class CConnectPoint : public CBillBoardImage
{
public:
	// コンストラクタ
	CConnectPoint();
	// デストラクタ
	~CConnectPoint();

	// 更新
	void Update() override;

private:
};