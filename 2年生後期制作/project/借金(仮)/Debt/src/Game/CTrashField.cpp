#include "CTrashField.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include "CVehicleManager.h"

CTrashField::CTrashField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	mpModel = CResourceManager::Get<CModel>("TrashStage");

	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eField, CResourceManager::Get<CModel>("TrashStage_Ground_Collision"), true);
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, CResourceManager::Get<CModel>("TrashStage_Wall_Collision"), true);
	mpObjectColliderMesh = new CColliderMesh(this, ELayer::eObject, CResourceManager::Get<CModel>("TrashStage_Object_Collision"), true);

	CreateFieldObjects();
}

CTrashField::~CTrashField()
{
	if (mpGroundColliderMesh != nullptr)
	{
		delete mpGroundColliderMesh;
		mpGroundColliderMesh = nullptr;
	}
	if (mpWallColliderMesh != nullptr)
	{
		delete mpWallColliderMesh;
		mpWallColliderMesh = nullptr;
	}
	if (mpObjectColliderMesh != nullptr)
	{
		delete mpObjectColliderMesh;
		mpObjectColliderMesh = nullptr;
	}



}

void CTrashField::CreateFieldObjects()
{
	mpCube = CResourceManager::Get<CModel>("FieldCube");
}

void CTrashField::Update()
{
}

void CTrashField::Render()
{
	mpModel->Render(Matrix());
}
