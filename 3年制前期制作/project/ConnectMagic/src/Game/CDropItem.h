#pragma once

class CObjectBase;

// 落とすアイテムのクラス
class CDropItem
{
public:
	// コンストラクタ
	CDropItem(CObjectBase* obj);
	// デストラクタ
	~CDropItem();

	// オブジェを取得
	CObjectBase* GetObj() const;

private:
	// 自身のオブジェクトクラスのポインタ
	CObjectBase* mpObj;
};