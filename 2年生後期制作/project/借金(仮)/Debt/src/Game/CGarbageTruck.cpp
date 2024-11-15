#include "CGarbageTruck.h"

CGarbageTruck::CGarbageTruck(CModel* model, const CVector& pos, const CVector& scale, const CVector& rotation)
	: CCar(model, pos, scale, rotation)
{
}

CGarbageTruck::~CGarbageTruck()
{
}

void CGarbageTruck::Update()
{
	CCar::Update();
}
