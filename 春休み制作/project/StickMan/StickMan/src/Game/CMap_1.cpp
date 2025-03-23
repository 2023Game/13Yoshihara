#include "CMap_1.h"

// コンストラクタ
CMap_1::CMap_1()
{
	mpModel = CResourceManager::Get<CModel>("Map_1");
}

// デストラクタ
CMap_1::~CMap_1()
{
}

// 更新
void CMap_1::Update()
{
}

// 描画
void CMap_1::Render()
{
	CFieldBase::Render();
}

// フィールドオブジェクトを生成
void CMap_1::CreateFieldObjects()
{
}

// 経路探索用のノードを生成
void CMap_1::CreateNavNodes()
{
}
