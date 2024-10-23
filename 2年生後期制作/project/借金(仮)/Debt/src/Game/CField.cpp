#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CComputer.h"

#define Scale_XYZ CVector(0.5f,0.5f,0.5f)

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	mpModel = CResourceManager::Get<CModel>("HomeBase");
	Scale(Scale_XYZ);

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	CreateFieldObjects();
}

CField::~CField()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CField::CreateFieldObjects()
{
	mpCube = CResourceManager::Get<CModel>("FieldCube");
	mpBed = CResourceManager::Get<CModel>("Bed");
	mpChair = CResourceManager::Get<CModel>("Chair");
	mpComputer = CResourceManager::Get<CModel>("Computer");
	mpDesk = CResourceManager::Get<CModel>("Desk");
	mpDoor = CResourceManager::Get<CModel>("Door");
	mpDustBox = CResourceManager::Get<CModel>("DustBox");
	mpRack = CResourceManager::Get<CModel>("Rack");

	new CComputer
	(
		mpComputer,
		CVector(20.5f, 7.6f, 10.0f),
		CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 180.0f, 0.0f)
	);
	//new CRotateFloor
	//(
	//	mpChair,
	//	CVector(-40.0f, 15.0f, 20.0f), CVector(1.0f, 1.0f, 1.0f),
	//	1.0f
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
	//	mpDustBox,
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

void CField::Update()
{
}

void CField::Render()
{
	mpModel->Render(Matrix());
}
