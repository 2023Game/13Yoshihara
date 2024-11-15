#include "CTrashField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CCar.h"
#include "CGarbageTruck.h"
#include "CCarAndTruckManager.h"

#define SCALE CVector(1.0f,1.0f,1.0f)
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
	Scale(SCALE);

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	CreateFieldObjects();
}

CTrashField::~CTrashField()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
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
