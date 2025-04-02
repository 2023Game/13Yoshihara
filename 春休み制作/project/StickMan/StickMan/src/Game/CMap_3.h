#pragma once
#include "CMapBase.h"

class CMap_3 : public CMapBase
{
public:
	// コンストラクタ
	CMap_3(bool isUp, bool isDown, bool isLeft, bool isRight);
	// デストラクタ
	~CMap_3();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// 経路探索用のノードを生成
	void CreateNavNodes() override;
};