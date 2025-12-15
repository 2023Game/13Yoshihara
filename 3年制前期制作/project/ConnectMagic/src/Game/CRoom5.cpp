#include "CRoom5.h"
#include "CWeight.h"
#include "CSwitch.h"
#include "CSwitchMoveWall.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CRespawnArea.h"
#include "CSaveManager.h"

// 部屋の長さ
constexpr float ROOM_LENGTH =			220.0f;

// 重りの座標
const CVector weight_OFFSET_POS1 =		CVector( 30.0f, 0.0f, -110.0f);
const CVector weight_OFFSET_POS2 =		CVector( 30.0f, 0.0f, -200.0f);

// 動く床のオフセット座標
const CVector MOVE_FLOOR_OFFSET_POS =	CVector( 0.0f, -10.0f, -40.0f);
// 動く床のスケール
const CVector MOVE_FLOOR_SCALE =		CVector( 4.0f,  1.0f,   4.0f);
// 動く床の移動
const CVector MOVE_FLOOR_MOVE =			CVector( 0.0f,  0.0f,  -100.0f);
// 動く床の移動時間
constexpr float MOVE_FLOOR_MOVE_TIME =	2.5f;

// スイッチのオフセット座標
const CVector SWITCH_OFFSET_POS =		CVector(-30.0f, 0.0f,  -110.0f);

// 動く壁のオフセット座標
const CVector MOVE_WALL_OFFSET_POS =	CVector( 0.0f,  0.0f,  -150.0f);
// 動く壁のスケール
const CVector MOVE_WALL_SCALE =			CVector( 8.0f,  4.0f,   1.0f);
// 動く壁の移動
const CVector MOVE_WALL_MOVE =			CVector(-80.0f, 0.0f,   0.0f);
// 動く壁の移動時間
constexpr float MOVE_WALL_MOVE_TIME =	0.5f;

// 水のオフセット座標
const CVector WATER_OFFSET_POS =		CVector( 0.0f, -5.0f,  -100.0f);
// 水のスケール
const CVector WATER_SCALE =				CVector( 50.0f, 1.0f,   100.0f);

// リスポーン地点
const CVector RESPAWN_OFFSET_POS =		CVector( 0.0f,  0.0f,  -30.0f);
// リスポーン地点の半径
constexpr float RESPAWN_RADIUS =		30.0f;

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

	// 保存管理クラス
	CSaveManager* saveMgr = CSaveManager::Instance();
	// 有効時のみ
	if (enable) {
		// 重りのフラグを変更
		mpWeight1->SetEnable(enable);
		mpWeight2->SetEnable(enable);
		// 重りを追加
		saveMgr->AddSavableObj(mpWeight1);
		saveMgr->AddSavableObj(mpWeight2);
		// 移動床を追加
		saveMgr->AddSavableObj(mpMoveFloor);
	}
	// 無効時
	else
	{
		// 重りがスイッチに張り付いていないなら
		if (!mpWeight1->GetIsAttach())
		{
			// 重りのフラグを変更
			mpWeight1->SetEnable(enable);
		}
		// 重りがスイッチに張り付いていないなら
		if (!mpWeight2->GetIsAttach())
		{
			// 重りのフラグを変更
			mpWeight2->SetEnable(enable);
		}
		// 重りを削除
		saveMgr->DeleteSavableObj(mpWeight1);
		saveMgr->DeleteSavableObj(mpWeight2);
		// 移動床を削除
		saveMgr->DeleteSavableObj(mpMoveFloor);
	}
}

// フィールドオブジェクトを生成
void CRoom5::CreateFieldObjects()
{
	// 重りを生成
	mpWeight1 = new CWeight(Position() + weight_OFFSET_POS1);
	mpWeight2 = new CWeight(Position() + weight_OFFSET_POS2);
	// 最初は無効
	mpWeight1->SetEnable(false);
	mpWeight2->SetEnable(false);

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
