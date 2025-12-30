#include "CRoom3.h"
#include "CWeight.h"
#include "CSwitch.h"
#include "CSwitchMoveFloor.h"
#include "CMoveObj.h"
#include "CWater.h"
#include "CSaveManager.h"

// 部屋の長さ
constexpr float ROOM_LENGTH =					240.0f;

// 重りの座標
const CVector weight_OFFSET_POS1 =				CVector( 160.0f,	 60.0f,	-220.0f);
const CVector weight_OFFSET_POS2 =				CVector( 80.0f,		 0.0f,	-100.0f);

// スイッチの座標
const CVector SWITCH_OFFSET_POS1 =				CVector( 10.0f,		 0.0f,	-30.0f);
const CVector SWITCH_OFFSET_POS2 =				CVector( 120.0f,	 60.0f,	-60.0f);

// スイッチで動く床のオフセット座標
const CVector SWITCH_MOVE_FLOOR_OFFSET_POS1 =	CVector( 80.0f,		 50.0f,	-40.0f);
const CVector SWITCH_MOVE_FLOOR_OFFSET_POS2 =	CVector( 0.0f,		-10.0f,	-40.0f);
// スイッチで動く床の移動
const CVector SWITCH_MOVE_FLOOR_MOVE1 =			CVector( 0.0f,		-60.0f,	 0.0f);
const CVector SWITCH_MOVE_FLOOR_MOVE2 =			CVector( 0.0f,		 0.0f,	-120.0f);

// 動く床のオフセット座標
const CVector MOVE_FLOOR_OFFSET_POS1 =			CVector( 120.0f,	-10.0f,	 0.0f);
const CVector MOVE_FLOOR_OFFSET_POS2 =			CVector( 160.0f,	-10.0f,	 0.0f);
const CVector MOVE_FLOOR_OFFSET_POS3 =			CVector( 160.0f,	 50.0f,	-160.0f);
// 動く床のスケール
const CVector MOVE_FLOOR_SCALE =				CVector( 4.0f,		 1.0f,	 4.0f);
// 動く床の移動
const CVector MOVE_FLOOR_MOVE1 =				CVector(-80.0f,		 0.0f,	 0.0f);
const CVector MOVE_FLOOR_MOVE2 =				CVector( 0.0f,		 0.0f,	-120.0f);
const CVector MOVE_FLOOR_MOVE3 =				CVector( 0.0f,		-60.0f,	 0.0f);
// 動く床の移動時間
constexpr float MOVE_FLOOR_MOVE_TIME =			2.5f;

// 水のオフセット座標
const CVector WATER_OFFSET_POS =				CVector( 80.0f,		-5.0f,	-120.0f);
// 水のスケール
const CVector WATER_SCALE =						CVector( 100.0f,	 1.0f,	 120.0f);

// リスポーン地点
const CVector RESPAWN_OFFSET_POS =				CVector( 0.0f,		 0.0f,	-30.0f);
// リスポーン地点の半径
constexpr float RESPAWN_RADIUS =				30.0f;

// コンストラクタ
CRoom3::CRoom3(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room3");
	// 座標を設定
	Position(pos);

	// コライダーを生成
	CreateCol("Room3_Col");

	// フィールドオブジェクトを生成
	CreateFieldObjects();
}

// デストラクタ
CRoom3::~CRoom3()
{
}

// 部屋の有効無効を設定
void CRoom3::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);
	mpSwitch1->SetEnableSwitch(enable);
	mpSwitch2->SetEnableSwitch(enable);

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
		saveMgr->AddSavableObj(mpSwitchMoveFloor1);
		saveMgr->AddSavableObj(mpSwitchMoveFloor2);
		saveMgr->AddSavableObj(mpMoveFloor1);
		saveMgr->AddSavableObj(mpMoveFloor2);
		saveMgr->AddSavableObj(mpMoveFloor3);
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
		saveMgr->DeleteSavableObj(mpSwitchMoveFloor1);
		saveMgr->DeleteSavableObj(mpSwitchMoveFloor2);
		saveMgr->DeleteSavableObj(mpMoveFloor1);
		saveMgr->DeleteSavableObj(mpMoveFloor2);
		saveMgr->DeleteSavableObj(mpMoveFloor3);
	}
}

// フィールドオブジェクトを生成
void CRoom3::CreateFieldObjects()
{
	// 重りを生成
	mpWeight1 = new CWeight(Position() + weight_OFFSET_POS1);
	mpWeight2 = new CWeight(Position() + weight_OFFSET_POS2);
	// 最初は無効
	mpWeight1->SetEnable(false);
	mpWeight2->SetEnable(false);

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
	// リストに追加
	mObjs.push_back(mpSwitchMoveFloor1);
	mObjs.push_back(mpSwitchMoveFloor2);
	// スイッチを設定
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
		ELayer::eField);
	mpMoveFloor2 = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS2,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE2,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eField);
	mpMoveFloor3 = new CMoveObj(model, colModel,
		Position() + MOVE_FLOOR_OFFSET_POS3,
		MOVE_FLOOR_SCALE,
		MOVE_FLOOR_MOVE3,
		MOVE_FLOOR_MOVE_TIME,
		ELayer::eField);
	// リストに追加
	mObjs.push_back(mpMoveFloor1);
	mObjs.push_back(mpMoveFloor2);
	mObjs.push_back(mpMoveFloor3);

	// 水を生成
	mpWater = new CWater(Position() + WATER_OFFSET_POS, WATER_SCALE);
	// リストに追加
	mObjs.push_back(mpWater);
}