#include "CRoom1.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CMoveFloor.h"

// 箱の座標
#define BOX_POS CVector(25.0f, 30.0f, 20.0f)
// スイッチの座標
#define SWITCH_POS CVector(25.0f, 0.0f, 5.0f)
// 部屋の長さ
#define ROOM_LENGTH 80.0f

// コンストラクタ
CRoom1::CRoom1(CVector pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room1");
	// 座標を設定
	Position(pos);

	// コライダーを生成
	CreateCol("Room1_Ground_Col", "Room1_Wall_Col", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CRoom1::~CRoom1()
{
}

// フィールドオブジェクトを生成
void CRoom1::CreateFieldObjects()
{
	// 箱を生成
	CBox* box = new CBox();
	// 座標を設定
	box->Position(Position() + BOX_POS);

	// スイッチを生成
	CSwitch* switchObj = new CSwitch(Position()+ SWITCH_POS);

	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CMoveFloor* moveFloor = new CMoveFloor(model, Position() + CVector(-60.0f, -10.0f, -90.0f), CVector(4.0f, 1.0f, 8.0f), CVector(30.0f,0.0f,0.0f), 5.0f);
}
