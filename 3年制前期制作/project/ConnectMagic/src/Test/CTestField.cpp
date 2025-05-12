#include "CTestField.h"
#include "CSwitch.h"
#include "CDoor.h"
#include "CBox.h"
#include "CAirConnectObj.h"

// スイッチの座標
#define SWITCH_POS	CVector(20.0f,0.0f,20.0f)
// ドアの座標
#define DOOR_POS	CVector(8.5f,0.0f,-42.5f)
// 箱の座標
#define BOX_POS		CVector(-25.0f,0.0f,-25.0f)

// 空中の接続オブジェクトの座標
#define AIR_POS_1	CVector(-2.5f,30.0f,-90.0f)

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

	// 箱を生成
	mpBox = new CBox();
	// 箱の位置調整
	mpBox->Position(BOX_POS);

	// 空中の接続オブジェクトを生成
	CAirConnectObj* airConnectObj = new CAirConnectObj();
	// 重力を掛けない
	airConnectObj->SetGravity(false);
	// 位置調整
	airConnectObj->Position(AIR_POS_1);
	// リストに追加
	mAirConnectObjs.push_back(airConnectObj);
}

// フィールドのコライダ―を生成
void CTestField::CreateCol()
{
	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eGround, CResourceManager::Get<CModel>("TutorialMap_Ground_Col"));
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, CResourceManager::Get<CModel>("TutorialMap_Wall_Col"));
}
