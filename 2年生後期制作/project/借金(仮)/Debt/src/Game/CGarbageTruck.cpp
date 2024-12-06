#include "CGarbageTruck.h"

CGarbageTruck::CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation)
	: CCar(model, pos, rotation)
{
}

CGarbageTruck::~CGarbageTruck()
{
}

void CGarbageTruck::Update()
{
	CCar::Move(GetStatusBase().mMoveSpeed);
}
