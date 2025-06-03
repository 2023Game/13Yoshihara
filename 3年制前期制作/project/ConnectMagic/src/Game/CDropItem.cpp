#include "CDropItem.h"

// コンストラクタ
CDropItem::CDropItem(CObjectBase* obj)
	: mpObj(obj)
{
}


// デストラクタ
CDropItem::~CDropItem()
{
}

// オブジェを取得
CObjectBase* CDropItem::GetObj() const
{
	return mpObj;
}
