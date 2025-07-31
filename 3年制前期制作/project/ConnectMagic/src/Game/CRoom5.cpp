#include "CRoom5.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"

// 部屋の長さ
#define ROOM_LENGTH 220.0f

// 箱の座標
#define BOX_OFFSET_POS1		CVector(30.0f,0.0f,-110.0f)
#define BOX_OFFSET_POS2		CVector(30.0f,0.0f,-200.0f)

// 動く床のオフセット座標
#define MOVE_FLOOR_OFFSET_POS	CVector(0.0f,-10.0f,-40.0f)
// 動く床のスケール
#define MOVE_FLOOR_SCALE		CVector(4.0f,1.0f,4.0f)
// 動く床の移動
#define MOVE_FLOOR_MOVE			CVector(0.0f,0.0f,-100.0f)
// 動く床の移動時間
#define MOVE_FLOOR_MOVE_TIME	2.5f

// スイッチのオフセット座標
#define SWITCH_OFFSET_POS	CVector(-30.0f,0.0f,-110.0f)

// 動く壁のオフセット座標
#define MOVE_WALL_OFFSET_POS	CVector(0.0f,0.0f,-150.0f)
// 動く壁のスケール
#define MOVE_WALL_SCALE			CVector(8.0f,4.0f,1.0f)
// 動く壁の移動
#define MOVE_WALL_MOVE			CVector(-80.0f,0.0f,0.0f)
// 動く壁の移動時間
#define MOVE_WALL_MOVE_TIME		0.5f

// 水のオフセット座標
#define WATER_OFFSET_POS	CVector(0.0f,-5.0f,-100.0f)
// 水のスケール
#define WATER_SCALE			CVector(50.0f,1.0f,100.0f)

// リスポーン地点
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// リスポーン地点の半径
#define RESPAWN_RADIUS	30.0f

// コンストラクタ
CRoom5::CRoom5(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room5");
	// 座標を設定
	Position(pos);

	// コライダーを生成
	CreateCol("Room5_Ground_Col", "Room5_Wall_Col", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CRoom5::~CRoom5()
{
}

// フィールドオブジェクトを生成
void CRoom5::CreateFieldObjects()
{
	// 箱を生成
	mpBox1 = new CBox(Position() + BOX_OFFSET_POS1);
	mpBox2 = new CBox(Position() + BOX_OFFSET_POS2);

	// 動く床を生成
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	mpMoveFloor = new CMoveObj(model,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);

	// スイッチを生成
	mpSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS);
	// 動く壁を生成
	mpMoveWall = new CSwitchMoveWall(model,
		Position() + MOVE_WALL_OFFSET_POS,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME);
	// 作用するオブジェクトに設定
	mpSwitch->SetActionObj(mpMoveWall);
	// 作用するスイッチに設定
	mpMoveWall->SetSwitchs({ mpSwitch });

	// 水を生成
	mpWater = new CWater(WATER_SCALE);
	// 座標を設定
	mpWater->Position(Position() + WATER_OFFSET_POS);

	// リスポーン地点
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
}

// フィールドオブジェクトを削除
void CRoom5::DeleteFieldObjects()
{
	mpBox1->Kill();
	mpBox2->Kill();
	mpMoveFloor->Kill();
	mpSwitch->DeleteSwitch();
	SAFE_DELETE(mpSwitch);
	mpMoveWall->Kill();
	mpWater->Kill();
	mpRespawnArea->Kill();
}
