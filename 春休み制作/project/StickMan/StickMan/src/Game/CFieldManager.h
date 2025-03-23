#pragma once
#include "CFieldBase.h"
#include <array>

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

private:
	// フィールド管理クラスのインスタンス
	static CFieldManager* spInstance;
	// マップの配列
	CFieldBase* mMapList[3][3];
};