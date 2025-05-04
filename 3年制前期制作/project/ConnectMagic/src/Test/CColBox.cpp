#include "CColBox.h"
#include "CColliderBox.h"
#include "Primitive.h"

CColBox::CColBox(CVector min, CVector max, const CColor defaultColor,
	bool isKinematic, float weight)
	: CColBase(defaultColor)
{
	mpCollider = new CColliderBox
	{
		this,ELayer::eTest,
		min,max,isKinematic,weight
	};
	mpCollider->SetCollisionLayers({ ELayer::eTest });
	mVertices = mpCollider->Get();
}

CColBox::~CColBox()
{
	SAFE_DELETE(mpCollider);
}

// è’ìÀîªíË
void CColBox::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CColBase::Collision(self, other, hit);

	Position(Position() + hit.adjust * hit.weight);
}

// ï`âÊ
void CColBox::Render()
{
	CMatrix m = Matrix();
	for (auto& v : mVertices)
	{
		Primitive::DrawTriangle
		(
			v.wv.V[0] * m,
			v.wv.V[1] * m,
			v.wv.V[2] * m,
			GetColor()
		);
		Primitive::DrawTriangle
		(
			v.wv.V[2] * m,
			v.wv.V[3] * m,
			v.wv.V[0] * m,
			GetColor()
		);
	}
}
