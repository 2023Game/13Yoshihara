#include "CRoom4.h"
#include "CWeight.h"
#include "CSwitch.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"
#include "CSaveManager.h"

// 部屋の長さ
#define ROOM_LENGTH 240.0f

// 重りの座標
#define WEIGHT_OFFSET_POS		CVector(30.0f,0.0f,-20.0f)

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

// 部屋の有効無効を設定
void CRoom4::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);

	// 保存管理クラス
	CSaveManager* saveMgr = CSaveManager::Instance();
	// 有効時のみ
	if (enable) {
		// 重りのフラグを変更
		mpWeight->SetEnable(enable);
		// 重りを追加
		saveMgr->AddWeight(mpWeight);
		// 移動床を追加
		saveMgr->AddMoveObj(mpMoveFloor);
		// 移動壁を追加
		saveMgr->AddMoveObj(mpMoveWall);
	}
	// 無効時
	else
	{
		// 重りがスイッチに張り付いていないなら
		if (!mpWeight->GetIsAttach())
		{
			// 重りのフラグを変更
			mpWeight->SetEnable(enable);
		}
		// 重りを削除
		saveMgr->DeleteWeight(mpWeight);
		// 移動床を削除
		saveMgr->DeleteMoveObj(mpMoveFloor);
		// 移動壁を削除
		saveMgr->DeleteMoveObj(mpMoveWall);
	}
}

// フィールドオブジェクトを生成
void CRoom4::CreateFieldObjects()
{
	// 重りを生成
	mpWeight = new CWeight(Position() + WEIGHT_OFFSET_POS);
	// 最初は無効
	mpWeight->SetEnable(false);

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

	// 動く壁を生成
	mpMoveWall= new CMoveObj(model, colModel,
		Position() + MOVE_WALL_OFFSET_POS,
		MOVE_WALL_SCALE,
		MOVE_WALL_MOVE,
		MOVE_WALL_MOVE_TIME,
		ELayer::eWall);
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
