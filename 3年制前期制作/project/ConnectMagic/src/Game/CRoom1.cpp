#include "CRoom1.h"
#include "CBox.h"
#include "CSwitch.h"
#include "CSwitchMoveFloor.h"
#include "CWater.h"
#include "CRespawnArea.h"
#include "CSaveManager.h"

// 部屋の長さ
#define ROOM_LENGTH 170.0f

// 箱の座標
#define BOX_OFFSET_POS CVector(25.0f, 0.0f, -150.0f)
// スイッチの座標
#define SWITCH_OFFSET_POS CVector(25.0f, 0.0f, -35.0f)

// 動く床のオフセット座標
#define MOVE_FLOOR_OFFSET_POS	CVector(-60.0f, -10.0f, -50.0f)
// 動く床のスケール
#define MOVE_FLOOR_SCALE		CVector(4.0f, 1.0f, 8.0f)
// 動く床の移動
#define MOVE_FLOOR_MOVE			CVector(30.0f,0.0f,0.0f)
// 動く床の移動時間	
#define MOVE_FLOOR_MOVE_TIME	1.0f

// 水のオフセット座標
#define WATER_OFFSET_POS	CVector(0.0f,-5.0f,-85.0f)
// 水のスケール
#define WATER_SCALE			CVector(50.0f,1.0f,50.0f)

// リスポーン地点
#define RESPAWN_OFFSET_POS		CVector(0.0f,0.0f,-30.0f)
// リスポーン地点の半径
#define RESPAWN_RADIUS	30.0f

// コンストラクタ
CRoom1::CRoom1(const CVector& pos)
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

// 部屋の有効無効を設定
void CRoom1::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);
	mpSwitch->SetEnableSwitch(enable);

	// 有効時のみ
	if (enable) {
		// 箱のフラグを変更
		mpBox->SetEnable(enable);
		// 保存管理クラス
		CSaveManager* saveMgr = CSaveManager::Instance();
		// 箱を追加
		saveMgr->AddBox(mpBox);
		// 移動床を追加
		saveMgr->AddMoveFloor(mpMoveFloor);
	}
}

// フィールドオブジェクトを生成
void CRoom1::CreateFieldObjects()
{
	// 箱を生成
	mpBox = new CBox(Position() + BOX_OFFSET_POS);
	// 最初は無効
	mpBox->SetEnable(false);

	// スイッチを生成
	mpSwitch = new CSwitch(Position()+ SWITCH_OFFSET_POS);
	// 動く床を生成
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CModel* colModel = CResourceManager::Get<CModel>("MoveObject_Col");
	mpMoveFloor = new CSwitchMoveFloor(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE,
		MOVE_FLOOR_MOVE_TIME);
	// 作用するスイッチに設定
	mpMoveFloor->SetSwitchs({ mpSwitch });
	// 作用するオブジェクトに設定
	mpSwitch->SetActionObj(mpMoveFloor);
	// リストに追加
	mObjs.push_back(mpMoveFloor);

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