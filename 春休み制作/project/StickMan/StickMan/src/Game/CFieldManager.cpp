#include "CFieldManager.h"
#include "CMap_1.h"

// フィールド管理クラスのインスタンス
CFieldManager* CFieldManager::spInstance = nullptr;

// フィールド管理クラスのインスタンスを取得
CFieldManager* CFieldManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CFieldManager::CFieldManager()
{
	spInstance = this;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; i < 3; i++)
		{
			mMapList[i][j] = new CMap_1();
		}
	}
}

// デストラクタ
CFieldManager::~CFieldManager()
{
	SAFE_DELETE(spInstance);
}
