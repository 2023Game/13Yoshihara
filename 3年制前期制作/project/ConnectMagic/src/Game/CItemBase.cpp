#include "CItemBase.h"

// �R���X�g���N�^
CItemBase::CItemBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mpCol(nullptr)
{
}

// �f�X�g���N�^
CItemBase::~CItemBase()
{
	SAFE_DELETE(mpCol);
}
