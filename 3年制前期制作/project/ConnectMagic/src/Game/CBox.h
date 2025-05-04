#pragma once
#include "CConnectObject.h"

class CBox : public CConnectObject
{
public:
	// コンストラクタ
	CBox();
	// デストラクタ
	~CBox();

	// 更新
	void Update() override;

	// 引っ張られた時の処理
	void Pull();

private:
	// コライダーを生成
	void CreateCol() override;
};