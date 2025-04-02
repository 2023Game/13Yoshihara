#include "CTable.h"

#define SCALE 1.5f

// コンストラクタ
CTable::CTable()
{
	mpModel = CResourceManager::Get<CModel>("Table");

	Scale(Scale() * SCALE);

	// コライダーを生成
	CreateCol("TableCol", "", "");
}

// デストラクタ
CTable::~CTable()
{
}

// フィールドオブジェクトを生成
void CTable::CreateFieldObjects()
{
}
