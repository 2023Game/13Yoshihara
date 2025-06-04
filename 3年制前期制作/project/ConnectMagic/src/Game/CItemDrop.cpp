#include "CItemDrop.h"
#include "CDropItem.h"

// �R���X�g���N�^
CItemDrop::CItemDrop()
	: mpDropItem(nullptr)
{
}

// �f�X�g���N�^
CItemDrop::~CItemDrop()
{
}

// ���Ƃ��A�C�e����ݒ�
void CItemDrop::SetDropItem(CDropItem* item)
{
	// ��Ȃ珈�����Ȃ�
	if (mpDropItem == nullptr) return;
	mpDropItem = item;
}

// ���Ƃ��A�C�e���̍��W��ݒ�
void CItemDrop::SetDropItemPos(CVector pos)
{
	// ��Ȃ珈�����Ȃ�
	if (mpDropItem == nullptr) return;
	// ���Ƃ��A�C�e���̍��W��ݒ�
	mpDropItem->GetObj()->Position(pos);
}

// �A�C�e���𗎂Ƃ�
void CItemDrop::Drop()
{
	// ��Ȃ珈�����Ȃ�
	if (mpDropItem == nullptr) return;
	mpDropItem->GetObj()->SetEnable(true);
	mpDropItem->GetObj()->SetShow(true);
}
