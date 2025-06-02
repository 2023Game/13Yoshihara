#include "CFragmentMergeArea.h"
#include "CColliderSphere.h"

// 半径
#define RADIUS 10.0f

// コンストラクタ
CFragmentMergeArea::CFragmentMergeArea()
	: CObjectBase(ETag::eFragmentMergeArea, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
{
	// コライダーを生成
	CreateCol();
}

// デストラクタ
CFragmentMergeArea::~CFragmentMergeArea()
{
}

// コライダーを生成
void CFragmentMergeArea::CreateCol()
{
	mpMergeAreaCol = new CColliderSphere(
		this, ELayer::eFragmentMergeArea,
		RADIUS, true
	);
	// オブジェクトとだけ衝突判定
	mpMergeAreaCol->SetCollisionLayers({ ELayer::eObject });
}
