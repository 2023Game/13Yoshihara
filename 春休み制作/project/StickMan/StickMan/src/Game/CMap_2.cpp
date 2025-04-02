#include "CMap_2.h"

// コンストラクタ
CMap_2::CMap_2(bool isUp, bool isDown, bool isLeft, bool isRight)
	: CMapBase(isUp, isDown, isLeft, isRight)
{
	mpModel = CResourceManager::Get<CModel>("Map_2");

	// コライダーを生成
	//CreateCol("Map_GroundCol", "Map_2_WallCol", "");
}

// デストラクタ
CMap_2::~CMap_2()
{
}

// フィールドオブジェクトを生成
void CMap_2::CreateFieldObjects()
{
}

// 経路探索用のノードを生成
void CMap_2::CreateNavNodes()
{
}
