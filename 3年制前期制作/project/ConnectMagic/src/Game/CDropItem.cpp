#include "CDropItem.h"

// �R���X�g���N�^
CDropItem::CDropItem(CObjectBase* obj)
	: mpObj(obj)
{
}


// �f�X�g���N�^
CDropItem::~CDropItem()
{
}

// �I�u�W�F���擾
CObjectBase* CDropItem::GetObj() const
{
	return mpObj;
}
