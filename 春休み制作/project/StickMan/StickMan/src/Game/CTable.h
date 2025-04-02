#pragma once
#include "CFieldBase.h"

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
};