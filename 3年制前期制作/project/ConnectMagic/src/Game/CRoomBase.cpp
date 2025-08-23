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

// 部屋の有効無効を切り替える
void CRoomBase::SetEnableRoom(bool enable)
{
	SetEnable(enable);
	// 全てのオブジェの有効無効を設定
	for (CObjectBase* obj : mObjs)
	{
		obj->SetEnable(enable);
	}
}
