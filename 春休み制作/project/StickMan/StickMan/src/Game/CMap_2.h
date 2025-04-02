#pragma once
#include "CMapBase.h"

class CMap_2 : public CMapBase
{
public:
	// コンストラクタ
	CMap_2(bool isUp, bool isDown, bool isLeft, bool isRight);
	// デストラクタ
	~CMap_2();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};