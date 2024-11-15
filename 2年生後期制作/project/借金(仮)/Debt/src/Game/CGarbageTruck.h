#pragma once
#include "CCar.h"

class CGarbageTruck : public CCar
{
public:
	CGarbageTruck(CModel* model, const CVector& pos, const CVector& scale, const CVector& rotation);
	~CGarbageTruck();

	void Update();
private:
};