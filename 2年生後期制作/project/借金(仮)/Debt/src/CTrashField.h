#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

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

	CModel* mpTrashBox;

	CModel* mpCube;

	TexAnimData mEffectAnimData;
};