#include "CHomeField.h"
#include "CCollisionManager.h"
#include "CComputer.h"
#include "CDoor.h"

#define SCALE CVector(0.5f,0.5f,0.5f)

// コンストラクタ
CHomeField::CHomeField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("HomeBase");
	Scale(SCALE);

	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eGround, CResourceManager::Get<CModel>("HomeBase_Ground&Ceiling_Collision"), true);
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, CResourceManager::Get<CModel>("HomeBase_Wall_Collision"), true);
	mpObjectColliderMesh = new CColliderMesh(this, ELayer::eObject, CResourceManager::Get<CModel>("HomeBase_Object_Collision"), true);

	CreateFieldObjects();
}

// デストラクタ
CHomeField::~CHomeField()
{
}

// フィールドのオブジェクト生成
void CHomeField::CreateFieldObjects()
{
	mpComputer = CResourceManager::Get<CModel>("Computer");
	mpDoor = CResourceManager::Get<CModel>("Door");

	CComputer* computer = new CComputer
	(
		mpComputer,
		CVector(20.5f, 7.6f, 10.0f),
		CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 180.0f, 0.0f)
	);

#if _DEBUG
	computer->SetDebugName("Computer");
#endif

	CDoor* door = new CDoor
	(
		mpDoor,
		CVector(23.5f, 0.0f, -14.0f),
		CVector(0.55f, 0.55f, 0.55f),
		CVector(0.0f, 180.0f, 0.0f)
	);
#if _DEBUG
	door->SetDebugName("Door");
#endif
}

// 更新
void CHomeField::Update()
{
}


void CHomeField::Render()
{
	CFieldBase::Render();
}
