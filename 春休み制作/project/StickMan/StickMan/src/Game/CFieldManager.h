#pragma once
#include "CMapBase.h"
#include <array>

// マップの行と列の数
#define MAP_ROW 3
#define MAP_COL 3

// フィールド管理クラス
class CFieldManager
{
public:
	// フィールド管理クラスのインスタンスを取得
	static CFieldManager* Instance();
	// コンストラクタ
	CFieldManager();
	// デストラクタ
	~CFieldManager();

	// 地面のコライダ―取得
	CCollider* GetGroundCol();
	// 壁のコライダ―取得
	CCollider* GetWallCol();
	// オブジェクトのコライダ―取得
	CCollider* GetObjCol();
private:
	// フィールド管理クラスのインスタンス
	static CFieldManager* spInstance;
	// マップの配列
	CMapBase* mpMapList[MAP_ROW][MAP_COL];

	// マップを生成
	void CreateMap();
};