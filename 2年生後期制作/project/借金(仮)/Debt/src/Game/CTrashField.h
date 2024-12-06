#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
class CVehicleManager;

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
	CColliderMesh* mpGroundColliderMesh;
	CColliderMesh* mpWallColliderMesh;
	CColliderMesh* mpObjectColliderMesh;

	CModel* mpCube;

	TexAnimData mEffectAnimData;
};