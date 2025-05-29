#pragma once
#include "CObjectBase.h"

// アイテムの基底クラス
class CItemBase : public CObjectBase
{
public:
	// コンストラクタ
	CItemBase(ETag tag = ETag::eItem, ETaskPriority prio = ETaskPriority::eBackground,
		int sortOrder = 0, ETaskPauseType pause = ETaskPauseType::eGame);
	// デストラクタ
	~CItemBase();

	// 取得時の処理
	virtual void GetItem() = 0;

private:
	// コライダー
	CCollider* mpCol;
};