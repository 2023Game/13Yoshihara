#pragma once
#include "CFieldBase.h"

class CMap3 : public CFieldBase
{
public:
	// コンストラクタ
	CMap3();
	// デストラクタ
	~CMap3();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};