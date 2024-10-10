#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpBed;
	CModel* mpChair;
	CModel* mpComputer;
	CModel* mpDesk;
	CModel* mpDoor;
	CModel* mpDustBox;
	CModel* mpRack;

	TexAnimData mEffectAnimData;
};