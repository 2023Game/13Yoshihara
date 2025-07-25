#include "CConnectRoom.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"

// 箱のオフセット座標
#define BOX_OFFSET_POS CVector(25.0f,0.0f,-40.0f)
// スイッチのオフセット座標
#define SWITCH_OFFSET_POS_NEXT	CVector(-25.0f,0.0f,-40.0f)
#define SWITCH_OFFSET_POS_PRE	CVector(25.0f,0.0f,-40.0f)
// 部屋の長さ
#define ROOM_LENGTH 80.0f

// 動く壁のオフセット座標
#define MOVE_WALL_OFFSET_POS_NEXT	CVector(0.0f,0.0f,-70.0f)
#define MOVE_WALL_OFFSET_POS_PRE	CVector(0.0f,0.0f,0.0f)
// 動く壁のスケール
#define MOVE_WALL_SCALE CVector(2.0f,4.0f,1.0f)
// 動く壁の移動
#define MOVE_WALL_MOVE	CVector(0.0f,40.0f,0.0f)
// 動く壁の移動時間
#define MOVE_WALL_MOVE_TIME 0.5f

// コンストラクタ
CConnectRoom::CConnectRoom(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("ConnectRoom");
	// 座標を設定
	Position(pos);

	// コライダーを生成
	CreateCol("ConnectRoom_Ground_Col", "ConnectRoom_Wall_Col", "");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CConnectRoom::~CConnectRoom()
{
}

// 次の部屋を設定
void CConnectRoom::SetNextRoom(CRoomBase* room)
{
	mpNextRoom = room;
}

// 前の部屋を設定
void CConnectRoom::SetPreRoom(CRoomBase* room)
{
	mpPreRoom = room;
}

// フィールドオブジェクトを生成
void CConnectRoom::CreateFieldObjects()
{
	// 箱を生成
	mpBox = new CBox(Position() + BOX_OFFSET_POS);

	// スイッチを生成
	mpNextSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS_NEXT);
	mpPreSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS_PRE);
	// 動く壁を生成
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	mpNextWall = new CSwitchMoveWall(model,model,
		Position() + MOVE_WALL_OFFSET_POS_NEXT, 
		MOVE_WALL_SCALE, 
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME);
	mpPreWall = new CSwitchMoveWall(model,model,
		Position() + MOVE_WALL_OFFSET_POS_PRE,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME);
	// 作用するスイッチに設定
	mpNextWall->SetSwitchs({ mpNextSwitch });
	mpPreWall->SetSwitchs({ mpPreSwitch });
	// 作用するオブジェクトに設定
	mpNextSwitch->SetActionObj(mpNextWall);
	mpPreSwitch->SetActionObj(mpPreWall);
}

// フィールドオブジェクトを削除
void CConnectRoom::DeleteFieldObjects()
{
	mpBox->Kill();
	mpNextSwitch->DeleteSwitch();
	SAFE_DELETE(mpNextSwitch);
	mpPreSwitch->DeleteSwitch();
	SAFE_DELETE(mpPreSwitch);
	mpNextWall->Kill();
	mpPreWall->Kill();
	mpNextRoom = nullptr;
	mpPreRoom = nullptr;
}
