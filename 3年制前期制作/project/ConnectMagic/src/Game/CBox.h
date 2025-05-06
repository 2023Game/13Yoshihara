#pragma once
#include "CConnectObject.h"

class CBox : public CConnectObject
{
public:
	// コンストラクタ
	CBox();
	// デストラクタ
	~CBox();

private:
	// コライダーを生成
	void CreateCol() override;
};