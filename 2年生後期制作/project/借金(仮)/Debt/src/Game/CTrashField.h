#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
class CCarAndTruckManager;

class CTrashField : public CObjectBase
{
public:
	CTrashField();
	~CTrashField();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCube;

	TexAnimData mEffectAnimData;
};