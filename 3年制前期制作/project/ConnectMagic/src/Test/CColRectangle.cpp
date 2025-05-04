#include "CColRectangle.h"
#include "Primitive.h"

// コンストラクタ
CColRectangle::CColRectangle(const CVector& v0, const CVector& v1, const CVector& v2, const CVector& v3,
	const CColor& defaultColor, bool isKinematic, float weight)
	: CColBase(defaultColor)
{
	mVertices[0] = v0;
	mVertices[1] = v1;
	mVertices[2] = v2;
	mVertices[3] = v3;

	mpCollider = new CColliderRectangle
	(
		this, ELayer::eTest,
		mVertices[0], mVertices[1], mVertices[2], mVertices[3],
		isKinematic, weight
	);
	mpCollider->SetCollisionLayers({ ELayer::eTest });
}

// デストラクタ
CColRectangle::~CColRectangle()
{
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CColRectangle::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CColBase::Collision(self, other, hit);

	Position(Position() + hit.adjust * hit.weight);
}

// 描画
void CColRectangle::Render()
{
	CMatrix m = Matrix();
	Primitive::DrawTriangle
	(
		mVertices[0] * m,
		mVertices[1] * m,
		mVertices[2] * m,
		GetColor()
	);
	Primitive::DrawTriangle
	(
		mVertices[2] * m,
		mVertices[3] * m,
		mVertices[0] * m,
		GetColor()
	);
}
