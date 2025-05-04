#include "CColMesh.h"
#include "Primitive.h"

CColMesh::CColMesh(CModel* model, const CColor& defaultColor,
	bool isKinematic, float weight)
	: CColBase(defaultColor)
{
	mpCollider = new CColliderMesh
	{
		this,ELayer::eTest,
		model,isKinematic,weight
	};
	mVertices = mpCollider->Get();
}

CColMesh::~CColMesh()
{
	SAFE_DELETE(mpCollider);
}

void CColMesh::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CColBase::Collision(self, other, hit);

	Position(Position() + hit.adjust * hit.weight);
}

void CColMesh::Render()
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
	}
}
