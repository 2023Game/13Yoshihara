#pragma once
#include "CRoomBase.h"

// 部屋1
class CRoom1 : public CRoomBase
{
public:
	// コンストラクタ
	CRoom1(CVector pos);
	// デストラクタ
	~CRoom1();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
};