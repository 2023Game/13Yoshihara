#include "CMap_1.h"

// コンストラクタ
CMap_1::CMap_1(bool isUp, bool isDown, bool isLeft, bool isRight)
	: CMapBase(isUp, isDown, isLeft, isRight)
{
	mpModel = CResourceManager::Get<CModel>("Book");

	// コライダーを生成
	//CreateCol("Map_GroundCol", "Map_1_WallCol", "");
}

// デストラクタ
CMap_1::~CMap_1()
{
}

// フィールドオブジェクトを生成
void CMap_1::CreateFieldObjects()
{
}

// 経路探索用のノードを生成
void CMap_1::CreateNavNodes()
{
}