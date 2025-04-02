#include "CClassRoom.h"

#define SCALE 10.0f

// コンストラクタ
CClassRoom::CClassRoom()
{
	mpModel = CResourceManager::Get<CModel>("ClassRoom");

	Scale(Scale() * SCALE);
}

// デストラクタ
CClassRoom::~CClassRoom()
{
}

// フィールドオブジェクトの生成
void CClassRoom::CreateFieldObjects()
{
}
