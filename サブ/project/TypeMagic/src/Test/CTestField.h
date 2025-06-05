#pragma once
#include "CFieldBase.h"

// テスト用のフィールド
class CTestField : public CFieldBase
{
public:
	// コンストラクタ
	CTestField();
	// デストラクタ
	~CTestField();
private:
	// フィールドのオブジェクトを生成
	void CreateFieldObjects() override;
};