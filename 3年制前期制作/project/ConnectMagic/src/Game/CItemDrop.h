#pragma once

class CDropItem;

// アイテムを落とすクラス
class CItemDrop
{
public:
	// コンストラクタ
	CItemDrop();
	// デストラクタ
	~CItemDrop();

	// 落とすアイテムを設定
	void SetDropItem(CDropItem* item);
	// 落とすアイテムの座標を設定
	void SetDropItemPos(CVector pos);

	// アイテムを落とす
	void Drop();

private:
	// 落とすアイテム
	CDropItem* mpDropItem;
};