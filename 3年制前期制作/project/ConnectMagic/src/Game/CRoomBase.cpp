#include "CRoomBase.h"

// コンストラクタ
CRoomBase::CRoomBase(float roomLength)
	: CFieldBase()
	, mRoomLength(roomLength)
{
}

// デストラクタ
CRoomBase::~CRoomBase()
{
}

// 部屋の長さを取得
float CRoomBase::GetRoomLength() const
{
	return mRoomLength;
}
