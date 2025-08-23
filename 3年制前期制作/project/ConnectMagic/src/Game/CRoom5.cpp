#include "CRoom5.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"
#include "CSaveManager.h"

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

// 部屋の有効無効を設定
void CRoom5::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);
	mpSwitch->SetEnableSwitch(enable);

	// 有効時のみ
	if (enable) {
		// 箱のフラグを変更
		mpBox1->SetEnable(enable);
		mpBox2->SetEnable(enable);
		// 保存管理クラス
		CSaveManager* saveMgr = CSaveManager::Instance();
		// 箱を追加
		saveMgr->AddBox(mpBox1);
		saveMgr->AddBox(mpBox2);
		// 移動床を追加
		saveMgr->AddMoveObj(mpMoveFloor);
	}
}

// フィールドオブジェクトを生成
void CRoom5::CreateFieldObjects()
{
	// 箱を生成
	mpBox1 = new CBox(Position() + BOX_OFFSET_POS1);
	mpBox2 = new CBox(Position() + BOX_OFFSET_POS2);
	// 最初は無効
	mpBox1->SetEnable(false);
	mpBox2->SetEnable(false);

	// 動く床を生成
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CModel* colModel = CResourceManager::Get<CModel>("MoveObject_Col");
	mpMoveFloor = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eGround);
	// リストに追加
	mObjs.push_back(mpMoveFloor);

	// スイッチを生成
	mpSwitch = new CSwitch(Position() + SWITCH_OFFSET_POS);
	// 動く壁を生成
	mpMoveWall = new CSwitchMoveWall(model, colModel,
		Position() + MOVE_WALL_OFFSET_POS,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME);
	// 作用するオブジェクトに設定
	mpSwitch->SetActionObj(mpMoveWall);
	// 作用するスイッチに設定
	mpMoveWall->SetSwitchs({ mpSwitch });
	// リストに追加
	mObjs.push_back(mpMoveWall);

	// 水を生成
	mpWater = new CWater(WATER_SCALE);
	// 座標を設定
	mpWater->Position(Position() + WATER_OFFSET_POS);
	// リストに追加
	mObjs.push_back(mpWater);

	// リスポーン地点
	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
	// リストに追加
	mObjs.push_back(mpRespawnArea);
}
