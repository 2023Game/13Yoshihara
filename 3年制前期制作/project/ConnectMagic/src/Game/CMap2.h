#pragma once
#include "CFieldBase.h"

// マップ2
class CMap2 : public CFieldBase
{
public:
	// コンストラクタ
	CMap2();
	// デストラクタ
	~CMap2();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
};