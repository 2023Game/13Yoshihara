#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CHomeField : public CObjectBase
{
public:
	CHomeField();
	~CHomeField();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpGroundColliderMesh;
	CColliderMesh* mpWallColliderMesh;
	CColliderMesh* mpObjectColliderMesh;

	CModel* mpBed;
	CModel* mpChair;
	CModel* mpComputer;
	CModel* mpDesk;
	CModel* mpDoor;
	CModel* mpRack;
	
	CModel* mpCube;

	TexAnimData mEffectAnimData;
};