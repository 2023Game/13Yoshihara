#pragma once
#include "CCharaBase.h"

class CCar : public CCharaBase
{
public:
	CCar();
	~CCar();

	void Update();
	void Collision();
	void Render();
private:
};