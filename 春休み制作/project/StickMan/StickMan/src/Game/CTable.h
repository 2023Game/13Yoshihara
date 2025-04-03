#pragma once
#include "CFieldBase.h"

class CMapBase;

// 机
class CTable : public CFieldBase
{
public:
	// コンストラクタ
	CTable();
	// デストラクタ
	~CTable();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects();

	// テーブル上のマップを生成
	CMapBase* mpMap;
};