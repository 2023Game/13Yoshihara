#include "CTestField.h"
#include "CSwitch.h"
#include "CDoor.h"

#define SWITCH_POS	CVector(25.0f,0.0f,25.0f)
#define DOOR_POS	CVector(8.5f,0.0f,-42.5f)

// コンストラクタ
CTestField::CTestField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TestField");

	// オブジェクトを生成
	CreateFieldObjects();
	// コライダ―を生成
	CreateCol();
}

// デストラクタ
CTestField::~CTestField()
{
}

// フィールドのオブジェクトを生成
void CTestField::CreateFieldObjects()
{
	// ドアを生成
	mpDoor = new CDoor(CVector::zero);
	// ドアの位置調整
	mpDoor->Position(DOOR_POS);

	// スイッチを生成
	mpSwitch = new CSwitch(SWITCH_POS);
	// ドアをスイッチの作用オブジェクトに設定
	mpSwitch->SetActionObj(mpDoor);
}

// フィールドのコライダ―を生成
void CTestField::CreateCol()
{
	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eGround, CResourceManager::Get<CModel>("TutorialMap_Ground_Col"));
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, CResourceManager::Get<CModel>("TutorialMap_Wall_Col"));
}
