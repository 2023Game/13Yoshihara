#pragma once
#include "CFieldBase.h"

class CHomeField : public CFieldBase
{
public:
	CHomeField();
	~CHomeField();

	void Update() override;
	void Render() override;

private:
	void CreateFieldObjects() override;

	CModel* mpComputer;
	CModel* mpDoor;
};