#include "CRoom2.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"
#include "CMoveFloor.h"
#include "CWater.h"
#include "CRespawnArea.h"

// 部屋の長さ
#define ROOM_LENGTH 220.0f

// 箱の座標
#define BOX_OFFSET_POS		CVector(30.0f,0.0f,-110.0f)
// スイッチの座標
#define SWITCH_OFFSET_POS	CVector(-30.0f,0.0f,-110.0f)

// 動く壁のオフセット座標
#define MOVE_WALL_OFFSET_POS	CVector(-5.0f,0.0f,-150.0f)
// 動く壁のスケール
#define MOVE_WALL_SCALE			CVector(9.0f,4.0f,1.0f)
// 動く壁の移動
#define MOVE_WALL_MOVE			CVector(-65.0f,0.0f,0.0f)
// 動く壁の移動時間
#define MOVE_WALL_MOVE_TIME		1.0f

// 動く床のオフセット座標
#define MOVE_FLOOR_OFFSET_POS	CVector(0.0f,-10.0f,-90.0f)
// 動く床のスケール
#define MOVE_FLOOR_SCALE		CVector(4.0f,1.0f,4.0f)
// 動く床の移動
#define MOVE_FLOOR_MOVE			CVector(0.0f,0.0f,50.0f)
// 動く床の移動時間
#define MOVE_FLOOR_MOVE_TIME	10.0f

// 水のオフセット座標
#define WATER_OFFSET_POS	CVector(0.0f,-5.0f,-85.0f)
// 水のスケール
#define WATER_SCALE			CVector(50.0f,1.0f,100.0f)

// リスポーン地点
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// リスポーン地点の半径
#define RESPAWN_RADIUS	30.0f

// コンストラクタ
CRoom2::CRoom2(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room2");
	// 座標を設定
	Position(pos);

	// コライダーを生成
	CreateCol("Room2_Ground_Col", "Room2_Wall_Col", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CRoom2::~CRoom2()
{
}

// フィールドオブジェクトを生成
void CRoom2::CreateFieldObjects()
{
	// 箱を生成
	mpBox = new CBox(Position() + BOX_OFFSET_POS);

	// スイッチを生成
	mpSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS);
	// 動く壁を生成
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	mpMoveWall = new CSwitchMoveWall(model,
		Position() + MOVE_WALL_OFFSET_POS,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME);
	// 作用するスイッチに設定
	mpMoveWall->SetSwitchs({ mpSwitch });
	// 作用するオブジェクトに設定
	mpSwitch->SetActionObj(mpMoveWall);

	// 動く床を生成
	mpMoveFloor = new CMoveFloor(model,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME);

	// 水を生成
	mpWater = new CWater(WATER_SCALE);
	// 座標を設定
	mpWater->Position(Position() + WATER_OFFSET_POS);

	// リスポーン地点
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
}

// フィールドオブジェクトを削除
void CRoom2::DeleteFieldObjects()
{
	mpBox->Kill();
	mpSwitch->DeleteSwitch();
	SAFE_DELETE(mpSwitch);
	mpMoveWall->Kill();
	mpMoveFloor->Kill();
	mpWater->Kill();
	mpRespawnArea->Kill();
}
