#pragma once
#include "CFieldBase.h"

class CMap_3 : public CFieldBase
{
public:
	// コンストラクタ
	CMap_3();
	// デストラクタ
	~CMap_3();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};