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

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// コライダーを生成
	void CreateCol() override;
	// 衝突判定
	CCollider* mpCol;
};