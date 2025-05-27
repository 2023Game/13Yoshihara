#pragma once
#include "CFieldBase.h"

class CMap1 : public CFieldBase
{
public:
	// コンストラクタ
	CMap1();
	// デストラクタ
	~CMap1();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};