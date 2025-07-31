#include "CRoom4.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"

// 部屋の長さ
#define ROOM_LENGTH 240.0f

// 箱の座標
#define BOX_OFFSET_POS		CVector(30.0f,0.0f,-20.0f)

// 動く床のオフセット座標
#define MOVE_FLOOR_OFFSET_POS	CVector(0.0f,-10.0f,-40.0f)
// 動く床のスケール
#define MOVE_FLOOR_SCALE		CVector(4.0f,1.0f,4.0f)
// 動く床の移動
#define MOVE_FLOOR_MOVE			CVector(0.0f,0.0f,-120.0f)
// 動く床の移動時間
#define MOVE_FLOOR_MOVE_TIME	2.0f

// 動く壁のオフセット座標
#define MOVE_WALL_OFFSET_POS	CVector(-25.0f,0.0f,-120.0f)
// 動く壁のスケール
#define MOVE_WALL_SCALE			CVector(3.0f,4.0f,1.0f)
// 動く壁の移動
#define MOVE_WALL_MOVE			CVector(50.0f,0.0f,0.0f)
// 動く壁の移動時間
#define MOVE_WALL_MOVE_TIME		2.5f

// 水のオフセット座標
#define WATER_OFFSET_POS	CVector(0.0f,-5.0f,-100.0f)
// 水のスケール
#define WATER_SCALE			CVector(50.0f,1.0f,100.0f)

// リスポーン地点
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// リスポーン地点の半径
#define RESPAWN_RADIUS	30.0f

// コンストラクタ
CRoom4::CRoom4(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room4");
	// 座標を設定
	Position(pos);

	// コライダーを生成
	CreateCol("Room4_Ground_Col", "Room4_Wall_Col", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CRoom4::~CRoom4()
{
}

// フィールドオブジェクトを生成
void CRoom4::CreateFieldObjects()
{
	// 箱を生成
	mpBox = new CBox(Position() + BOX_OFFSET_POS);

	// 動く床を生成
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	mpMoveFloor = new CMoveObj(model,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);

	// 動く壁を生成
	mpMoveWall= new CMoveObj(model,
		Position() + MOVE_WALL_OFFSET_POS,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME,
		ELayer::eWall);

	// 水を生成
	mpWater = new CWater(WATER_SCALE);
	// 座標を設定
	mpWater->Position(Position() + WATER_OFFSET_POS);

	// リスポーン地点
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
}

// フィールドオブジェクトを削除
void CRoom4::DeleteFieldObjects()
{
	mpBox->Kill();
	mpMoveFloor->Kill();
	mpWater->Kill();
	mpRespawnArea->Kill();
}
