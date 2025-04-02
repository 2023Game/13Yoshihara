#include "CFieldManager.h"
#include "CMap_1.h"

// マップ同士の間隔
#define MAP_DIST 200.0f

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

	// マップを生成
	CreateMap();
}

// デストラクタ
CFieldManager::~CFieldManager()
{
	SAFE_DELETE(spInstance);
}

// 地面のコライダ―取得
CCollider* CFieldManager::GetGroundCol()
{
	return nullptr;
}

// 壁のコライダ―取得
CCollider* CFieldManager::GetWallCol()
{
	return nullptr;
}

// オブジェクトのコライダ―取得
CCollider* CFieldManager::GetObjCol()
{
	return nullptr;
}

// マップを生成
void CFieldManager::CreateMap()
{
	// マップ同士が繋がれるかどうか
	bool isUp = true;
	bool isDown = true;
	bool isLeft = true;
	bool isRight = true;
	for (int i = 0; i < 3; i++)
	{
		// 一番上の行なら上へは繋がらない
		if (i == 0)
		{
			isUp = false;
			isDown = true;
		}
		// 一番下の行なら下へは繋がらない
		else if (i == MAP_ROW - 1)
		{
			isUp = true;
			isDown = false;
		}
		// それ以外は繋がる
		else
		{
			isUp = true;
			isDown = true;
		}
		for (int j = 0; j < 3; j++)
		{
			// 一番左の列なら左へは繋がらない
			if (j == 0)
			{
				isLeft = false;
				isRight = true;
			}
			// 一番右の列なら右へは繋がらない
			else if (j == MAP_COL - 1)
			{
				isLeft = true;
				isRight = false;
			}
			// それ以外は繋がる
			else
			{
				isLeft = true;
				isRight = true;
			}
			// マップを生成
			mpMapList[i][j] = new CMap_1(isUp, isDown, isLeft, isRight);
			// 座標を設定
			mpMapList[i][j]->Position((j - 1) * MAP_DIST, 0.0f, (i - 1) * MAP_DIST);
			// 塞ぐ壁を生成
			mpMapList[i][j]->CreateBlockWall();
		}
	}
}
