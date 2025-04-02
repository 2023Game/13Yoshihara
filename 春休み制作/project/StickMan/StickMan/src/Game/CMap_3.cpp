#include "CMap_3.h"

// コンストラクタ
CMap_3::CMap_3(bool isUp, bool isDown, bool isLeft, bool isRight)
	: CMapBase(isUp, isDown, isLeft, isRight)
{
	mpModel = CResourceManager::Get<CModel>("Map_3");

	// コライダーを生成
	CreateCol("Map_3_GroundCol", "Map_3_WallCol", "");
}

// デストラクタ
CMap_3::~CMap_3()
{
}

// フィールドオブジェクトを生成
void CMap_3::CreateFieldObjects()
{
}

// 経路探索用のノードを生成
void CMap_3::CreateNavNodes()
{
}
