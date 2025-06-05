#include "CTutorialMap.h"

// コンストラクタ
CTutorialMap::CTutorialMap()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TutorialMap");

	// オブジェクトを生成
	CreateFieldObjects();
	// コライダ―を生成
	CreateCol("TutorialMap_Ground_Col", "TutorialMap_Wall_Col", "");
}

// デストラクタ
CTutorialMap::~CTutorialMap()
{
}

// フィールドのオブジェクトを生成
void CTutorialMap::CreateFieldObjects()
{
}