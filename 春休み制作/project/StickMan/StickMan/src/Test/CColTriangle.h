#pragma once
#include "CColBase.h"
#include "CColliderTriangle.h"

class CColTriangle : public CColBase
{
public:
	// コンストラクタ
	CColTriangle(const CVector& v0, const CVector& v1, const CVector& v2, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// デストラクタ
	~CColTriangle();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render() override;

private:
	// 三角形コライダー
	CColliderTriangle* mpCollider;
	CVector mVertices[3];	// 三角形を構成する3頂点
};