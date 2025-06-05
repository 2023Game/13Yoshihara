#pragma once
#include "CColBase.h"
#include "CColliderRectangle.h"

class CColRectangle : public CColBase
{
public:
	// コンストラクタ
	CColRectangle(const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
		const CColor& defaultColor, bool isKinematic = false, float weight = 1.0f);
	// デストラクタ
	~CColRectangle();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self,CCollider* other,const CHitInfo& hit) override;

	// 描画
	void Render() override;

private:
	// 四角形のコライダ―
	CColliderRectangle* mpCollider;
	// 四角形を構成する4頂点
	CVector mVertices[4];
};