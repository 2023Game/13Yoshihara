#include "CItemBase.h"

// コンストラクタ
CItemBase::CItemBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mpCol(nullptr)
{
}

// デストラクタ
CItemBase::~CItemBase()
{
	SAFE_DELETE(mpCol);
}
