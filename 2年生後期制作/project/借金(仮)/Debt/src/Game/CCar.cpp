#include "CCar.h"
#include "CModel.h"

#define MOVE_SPEED 

CCar::CCar(CModel* model, const CVector& pos, const CVector& scale, const CVector& rotation)
	: CCharaBase(ETag::eCar, ETaskPriority::eDefault)
	, mpModel(model)
{
	Position(pos);
	Scale(scale);
	Rotation(rotation);
}

CCar::~CCar()
{
}

void CCar::Update()
{
	//Position(Position() + VectorZ() * 2.0f);
}

void CCar::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

void CCar::Render()
{
	mpModel->Render(Matrix());
}
