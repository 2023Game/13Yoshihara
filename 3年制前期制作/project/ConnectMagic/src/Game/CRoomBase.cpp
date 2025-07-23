#include "CRoomBase.h"

// コンストラクタ
CRoomBase::CRoomBase(float roomLength)
	: CFieldBase()
{
}

// デストラクタ
CRoomBase::~CRoomBase()
{
}

// 部屋の長さを取得
float CRoomBase::SetRoomLength() const
{
	return mRoomLength;
}
