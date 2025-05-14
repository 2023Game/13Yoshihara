#pragma once
#include "CBillBoardImage.h"
#include "CColliderSphere.h"

// ポータルのエフェクト
class CPortal : public CBillBoardImage
{
public:
	// コンストラクタ
	CPortal();
	// デストラクタ
	~CPortal();

private:
	// コライダーを生成
	void CreateCol() override;
	// 衝突判定
	CCollider* mpCol;
};