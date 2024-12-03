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
	CColliderMesh* mpWallColliderMesh;
	CColliderMesh* mpGroundColliderMesh;
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