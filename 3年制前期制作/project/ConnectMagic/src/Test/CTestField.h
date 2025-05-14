#pragma once
#include "CFieldBase.h"

class CSwitch;
class CDoor;
class CBox;
class CAirConnectObj;

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
	// フィールドのコライダ―を生成
	void CreateCol() override;
};