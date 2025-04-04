#pragma once
#include "CFieldBase.h"

class CMap_1 : public CFieldBase
{
public:
	// コンストラクタ
	CMap_1();
	// デストラクタ
	~CMap_1();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};