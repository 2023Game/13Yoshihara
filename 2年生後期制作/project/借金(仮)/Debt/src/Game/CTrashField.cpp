#include "CTrashField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CCar.h"

#define SCALE CVector(1.0f,1.0f,1.0f)
#define CAR_RIGHT_POS1 CVector(60.0f,0.0f,0.0f)
#define CAR_RIGHT_POS2 CVector(20.0f,0.0f,0.0f)
#define CAR_RIGHT_ROTATION CVector(0.0f,.0f,0.0f)
#define CAR_LEFT_POS1 CVector(-60.0f,0.0f,0.0f)
#define CAR_LEFT_POS2 CVector(-20.0f,0.0f,0.0f)
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

	mpTrashBox = CResourceManager::Get<CModel>("TrashBox");
	mpCar = CResourceManager::Get<CModel>("Car");


	// ˆê”Ô‰E‚ÌŽÔ
	new CCar
	(
		mpCar,
		CAR_RIGHT_POS1,
		SCALE,
		CAR_RIGHT_ROTATION
	);
	// ‰E‚©‚ç2”Ô–Ú‚ÌŽÔ
	new CCar
	(
		mpCar,
		CAR_RIGHT_POS2,
		SCALE,
		CAR_RIGHT_ROTATION
	);
	// ˆê”Ô¶‚ÌŽÔ
	new CCar
	(
		mpCar,
		CAR_LEFT_POS1,
		SCALE,
		CAR_LEFT_ROTATION
	);
	// ¶‚©‚ç2”Ô–Ú‚ÌŽÔ
	new CCar
	(
		mpCar,
		CAR_LEFT_POS2,
		SCALE,
		CAR_LEFT_ROTATION
	);


	//new CComputer
	//(
	//	mpComputer,
	//	CVector(20.5f, 7.6f, 10.0f),
	//	CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 180.0f, 0.0f)
	//);
	//new CDoor
	//(
	//	mpDoor,
	//	CVector(23.5f, 0.0f, -14.0f),
	//	CVector(0.55f, 0.55f, 0.55f),
	//	CVector(0.0f, 180.0f, 0.0f)
	//);

	//// “®‚©‚È‚¢°‡@
	//new CMoveFloor
	//(
	//	mpComputer,
	//	CVector(20.0f, 10.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// “®‚­°‡@
	//new CMoveFloor
	//(
	//	mpDesk,
	//	CVector(60.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// “®‚©‚È‚¢°‡A
	//new CMoveFloor
	//(
	//	mpTrashBox,
	//	CVector(100.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// ‰ñ“]‚·‚é°‡@
	//new CRotateFloor
	//(
	//	mpRack,
	//	CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
	//	0.0f
	//);
	//// “®‚©‚È‚¢°‡A
	//new CMoveFloor
	//(
	//	mpDoor,
	//	CVector(135.0f, 20.0f, -35.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// “®‚©‚È‚¢°‡A
	//new CMoveFloor
	//(
	//	mpDesk,
	//	CVector(135.0f, 70.0f, -52.5f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 50.0f, 0.0f), 5.0f
	//);
	//// “®‚©‚È‚¢°‡Biâ“¹j
	//CMoveFloor* mf = new CMoveFloor
	//(
	//	mpCube,
	//	CVector(0.0f, 20.0f, 200.5f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//mf->Rotate(0.0f, 0.0f, 30.0f);

	// “dŒ‚ƒGƒtƒFƒNƒg
	//CLineEffect* le = new CLineEffect(ETag::eField);
	//le->SetTexture("LightningBolt");
	//le->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	//le->SetAnimData(&mEffectAnimData);

	//CVector startPos = CVector(50.0f, 10.0f, 0.0f);
	//CVector endPos = CVector(50.0f, 10.0f, 150.0f);
	//int div = 3;
	//float width = 5.0f;
	//le->AddPoint(startPos, width, width);
	//for (int i = 0; i < div; i++)
	//{
	//	float alpha = (float)(i + 1) / div;
	//	CVector pos = CVector::Lerp(startPos, endPos, alpha);
	//	le->AddPoint(pos, width, width);
	//}
}

void CTrashField::Update()
{
}

void CTrashField::Render()
{
	mpModel->Render(Matrix());
}
