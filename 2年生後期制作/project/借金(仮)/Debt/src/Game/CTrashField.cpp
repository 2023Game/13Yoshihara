#include "CTrashField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include "CCarAndTruckManager.h"

#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,-100.0f)
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,-100.0f)
#define CAR_RIGHT_ROTATION CVector(0.0f,.0f,0.0f)
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,100.0f)
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,100.0f)
#define CAR_LEFT_ROTATION CVector(0.0f,180.0f,0.0f)

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
