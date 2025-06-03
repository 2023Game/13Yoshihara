#include "CItemDrop.h"
#include "CDropItem.h"

// コンストラクタ
CItemDrop::CItemDrop()
	: mpDropItem(nullptr)
{
}

// デストラクタ
CItemDrop::~CItemDrop()
{
}

// 落とすアイテムを設定
void CItemDrop::SetDropItem(CDropItem* item)
{
	mpDropItem = item;
}

// 落とすアイテムの座標を設定
void CItemDrop::SetDropItemPos(CVector pos)
{
	// 落とすアイテムの座標を設定
	mpDropItem->GetObj()->Position(pos);
}

// アイテムを落とす
void CItemDrop::Drop()
{
	mpDropItem->GetObj()->SetEnable(true);
	mpDropItem->GetObj()->SetShow(true);
}
