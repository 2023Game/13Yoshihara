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
	// 空なら処理しない
	if (mpDropItem == nullptr) return;
	mpDropItem = item;
}

// 落とすアイテムの座標を設定
void CItemDrop::SetDropItemPos(CVector pos)
{
	// 空なら処理しない
	if (mpDropItem == nullptr) return;
	// 落とすアイテムの座標を設定
	mpDropItem->GetObj()->Position(pos);
}

// アイテムを落とす
void CItemDrop::Drop()
{
	// 空なら処理しない
	if (mpDropItem == nullptr) return;
	mpDropItem->GetObj()->SetEnable(true);
	mpDropItem->GetObj()->SetShow(true);
}
