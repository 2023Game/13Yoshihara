#include "CTable.h"
#include "CMap_1.h"

#define SCALE 1.5f
#define ROT	CVector(0.0f,90.0f,0.0f)

// コンストラクタ
CTable::CTable()
{
	mpModel = CResourceManager::Get<CModel>("Table");

	Scale(Scale() * SCALE);
	Rotation(ROT);

	// コライダーを生成
	CreateCol("TableCol", "", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CTable::~CTable()
{
}

// フィールドオブジェクトを生成
void CTable::CreateFieldObjects()
{
	mpMap = new CMap_1(true, true, true, true);

	// 繋ぐ壁を生成
	mpMap->CreateConnectWall();
}
