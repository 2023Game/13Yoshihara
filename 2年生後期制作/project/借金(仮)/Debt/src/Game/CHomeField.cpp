#include "CHomeField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CComputer.h"
#include "CDoor.h"

#define Scale_XYZ CVector(0.5f,0.5f,0.5f)

CHomeField::CHomeField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	mpModel = CResourceManager::Get<CModel>("HomeBase");
	Scale(Scale_XYZ);

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	CreateFieldObjects();
}

CHomeField::~CHomeField()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CHomeField::CreateFieldObjects()
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
	new CDoor
	(
		mpDoor,
		CVector(23.5f, 0.0f, -14.0f),
		CVector(0.55f, 0.55f, 0.55f),
		CVector(0.0f, 180.0f, 0.0f)
	);

	//// 動かない床�@
	//new CMoveFloor
	//(
	//	mpComputer,
	//	CVector(20.0f, 10.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// 動く床�@
	//new CMoveFloor
	//(
	//	mpDesk,
	//	CVector(60.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// 動かない床�A
	//new CMoveFloor
	//(
	//	mpDustBox,
	//	CVector(100.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// 回転する床�@
	//new CRotateFloor
	//(
	//	mpRack,
	//	CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
	//	0.0f
	//);
	//// 動かない床�A
	//new CMoveFloor
	//(
	//	mpDoor,
	//	CVector(135.0f, 20.0f, -35.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// 動かない床�A
	//new CMoveFloor
	//(
	//	mpDesk,
	//	CVector(135.0f, 70.0f, -52.5f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 50.0f, 0.0f), 5.0f
	//);
	//// 動かない床�B（坂道）
	//CMoveFloor* mf = new CMoveFloor
	//(
	//	mpCube,
	//	CVector(0.0f, 20.0f, 200.5f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//mf->Rotate(0.0f, 0.0f, 30.0f);

	// 電撃エフェクト
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

void CHomeField::Update()
{
}

void CHomeField::Render()
{
	mpModel->Render(Matrix());
}
