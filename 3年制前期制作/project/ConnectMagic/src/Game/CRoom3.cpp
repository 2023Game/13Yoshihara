#include "CRoom3.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveFloor.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"

// 部屋の長さ
#define ROOM_LENGTH 240.0f

// 箱の座標
#define BOX_OFFSET_POS1	CVector(160.0f,60.0f,-220.0f)
#define BOX_OFFSET_POS2	CVector(80.0f,0.0f,-100.0f)

// スイッチの座標
#define SWITCH_OFFSET_POS1	CVector(10.0f,0.0f,-30.0f)
#define SWITCH_OFFSET_POS2	CVector(120.0f,60.0f,-60.0f)

// スイッチで動く床のオフセット座標
#define SWITCH_MOVE_FLOOR_OFFSET_POS1	CVector(80.0f,50.0f,-40.0f)
#define SWITCH_MOVE_FLOOR_OFFSET_POS2	CVector(0.0f,-10.0f,-40.0f)
// スイッチで動く床の移動
#define SWITCH_MOVE_FLOOR_MOVE1			CVector(0.0f,-60.0f,0.0f)
#define SWITCH_MOVE_FLOOR_MOVE2			CVector(0.0f,0.0f,-120.0f)

// 動く床のオフセット座標
#define MOVE_FLOOR_OFFSET_POS1	CVector(120.0f,-10.0f,0.0f)
#define MOVE_FLOOR_OFFSET_POS2	CVector(160.0f,-10.0f,0.0f)
#define MOVE_FLOOR_OFFSET_POS3	CVector(160.0f,50.0f,-160.0f)
// 動く床のスケール
#define MOVE_FLOOR_SCALE		CVector(4.0f,1.0f,4.0f)
// 動く床の移動
#define MOVE_FLOOR_MOVE1		CVector(-80.0f,0.0f,0.0f)
#define MOVE_FLOOR_MOVE2		CVector(0.0f,0.0f,-120.0f)
#define MOVE_FLOOR_MOVE3		CVector(0.0f,-60.0f,0.0f)
// 動く床の移動時間
#define MOVE_FLOOR_MOVE_TIME	2.5f

// 水のオフセット座標
#define WATER_OFFSET_POS	CVector(80.0f,-5.0f,-120.0f)
// 水のスケール
#define WATER_SCALE			CVector(100.0f,1.0f,120.0f)

// リスポーン地点
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// リスポーン地点の半径
#define RESPAWN_RADIUS	30.0f

// コンストラクタ
CRoom3::CRoom3(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room3");
	// 座標を設定
	Position(pos);

	// コライダーを生成
	CreateCol("Room3_Ground_Col", "Room3_Wall_Col", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CRoom3::~CRoom3()
{
}

// フィールドオブジェクトを生成
void CRoom3::CreateFieldObjects()
{
	// 箱を生成
	mpBox1 = new CBox(Position() + BOX_OFFSET_POS1);
	mpBox2 = new CBox(Position() + BOX_OFFSET_POS2);

	// スイッチを生成
	mpSwitch1 = new CSwitch(Position() + SWITCH_OFFSET_POS1);
	mpSwitch2 = new CSwitch(Position() + SWITCH_OFFSET_POS2);

	// スイッチで動く床を生成
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CModel* colModel = CResourceManager::Get<CModel>("MoveObject_Col");
	mpSwitchMoveFloor1 = new CSwitchMoveFloor(model, colModel,
		Position() + SWITCH_MOVE_FLOOR_OFFSET_POS1,
		MOVE_FLOOR_SCALE,
		SWITCH_MOVE_FLOOR_MOVE1,
		MOVE_FLOOR_MOVE_TIME);
	mpSwitchMoveFloor2 = new CSwitchMoveFloor(model, colModel,
		Position() + SWITCH_MOVE_FLOOR_OFFSET_POS2,
		MOVE_FLOOR_SCALE,
		SWITCH_MOVE_FLOOR_MOVE2,
		MOVE_FLOOR_MOVE_TIME);
	mpSwitch1->SetActionObj(mpSwitchMoveFloor1);
	mpSwitch2->SetActionObj(mpSwitchMoveFloor2);
	mpSwitchMoveFloor1->SetSwitchs({ mpSwitch1 });
	mpSwitchMoveFloor2->SetSwitchs({ mpSwitch2 });

	// 動く床を生成
	mpMoveFloor1 = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS1,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE1,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);
	mpMoveFloor2 = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS2,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE2,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);
	mpMoveFloor3 = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS3,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE3,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);

	// 水を生成
	mpWater = new CWater(WATER_SCALE);
	// 座標を設定
	mpWater->Position(Position() + WATER_OFFSET_POS);

	// リスポーン地点
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
}

// フィールドオブジェクトを削除
void CRoom3::DeleteFieldObjects()
{
}
