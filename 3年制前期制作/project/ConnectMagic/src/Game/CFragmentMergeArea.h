#pragma once
#include "CObjectBase.h"

// 欠片を合成する場所
class CFragmentMergeArea : public CObjectBase
{
public:
	// コンストラクタ
	CFragmentMergeArea();
	// デストラクタ
	~CFragmentMergeArea();

private:
	// コライダーを生成
	void CreateCol() override;
	// 欠片を合成する場所のコライダー
	CCollider* mpMergeAreaCol;
};