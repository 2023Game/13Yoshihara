#pragma once
#include "CFieldBase.h"

class CMap_2 : public CFieldBase
{
public:
	// コンストラクタ
	CMap_2();
	// デストラクタ
	~CMap_2();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};