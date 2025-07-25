#pragma once
#include "CFieldBase.h"

// 部屋の基底クラス
class CRoomBase : public CFieldBase
{
public:
	// コンストラクタ
	CRoomBase(float roomLength);
	// デストラクタ
	~CRoomBase();

	// 部屋の長さを取得
	float GetRoomLength() const;

private:
	// 部屋の長さ
	float mRoomLength;
};