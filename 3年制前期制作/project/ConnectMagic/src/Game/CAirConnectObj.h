#pragma once
#include "CConnectObject.h"

// 空中に浮かんでいる接続オブジェクト
class CAirConnectObj : public CConnectObject
{
public:
	// コンストラクタ
	CAirConnectObj();
	// デストラクタ
	~CAirConnectObj();

private:
	// コライダーを生成
	void CreateCol() override;
};