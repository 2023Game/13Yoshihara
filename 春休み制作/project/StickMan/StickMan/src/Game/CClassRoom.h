#pragma once
#include "CFieldBase.h"

// 教室
class CClassRoom : public CFieldBase
{
public:
	// コンストラクタ
	CClassRoom();
	// デストラクタ
	~CClassRoom();

private:
	// フィールドオブジェクトの生成
	void CreateFieldObjects();
};