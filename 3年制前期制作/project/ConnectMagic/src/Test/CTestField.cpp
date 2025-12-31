#include "CTestField.h"
#include "CSwitch.h"
#include "CWeight.h"
#include "CAirConnectObj.h"
#include "CPortal.h"
#include "CWater.h"
#include "CConnectRoom.h"
#include "CRoom1.h"
#include "CSwitchMoveFloor.h"
#include "CSwitchMoveAirObj.h"
#include "CSwitchPushWall.h"
#include "CSwitchMoveWall.h"
#include "CSwitchShield.h"
#include "CShield.h"
#include "CPhysicsManager.h"

// 水の座標
const CVector WATER_POS =						CVector( 300.0f, 5.0f, 0.0f);
const CVector WATER_SCALE =						CVector( 100.0f, 1.0f, 100.0f);
// 重りの座標
const CVector WEIGHT_POS =						CVector( 50.0f, 0.0f, -25.0f);
// 空中のコネクトオブジェクト
const CVector AIR_CONNECT_POS =					CVector( 0.0f, 40.0f, 0.0f);
// ポータルの設定
const CVector PORTAL_POS =						CVector( 100.0f, 10.0f, -100.0f);
// リスポーン地点の設定
const CVector RESPAWN_POS_1 =					CVector( 0.0f, -5.0f,  -180.0f);
const CVector RESPAWN_POS_2 =					CVector(-90.0f, 35.0f, -430.0f);
constexpr float RESPAWN_RADIUS =				25.0f;
// クリスタル
const CVector CRYSTAL_POS1 =					CVector( 20.0f, 0.0f, 0.0f);
const CVector CRYSTAL_POS2 =					CVector(-20.0f, 0.0f, 0.0f);
const CVector CRYSTAL_POS3 =					CVector(-40.0f, 0.0f, 0.0f);
// スイッチで動く空中コネクトオブジェクト
const CVector SWITCH_AIR_OBJ_POS =				CVector( 40.0f, 40.0f, 0.0f);
const CVector SWITCH_AIR_OBJ_MOVE_POS1 =		CVector( 40.0f, 40.0f, 0.0f);
const CVector SWITCH_AIR_OBJ_MOVE_POS2 =		CVector( 80.0f, 80.0f, 0.0f);
// 押し出す壁
const CVector SWITCH_PUSH_WALL_POS =			CVector( 80.0f,  0.0f, 0.0f);
const CVector SWITCH_PUSH_WALL_SCALE =			CVector( 1.0f,   4.0f, 1.0f);
const CVector SWITCH_PUSH_WALL_MOVE =			CVector( 100.0f, 0.0f, 0.0f);
constexpr float SWITCH_PUSH_WALL_MOVE_TIME =	12.0f;
// スイッチで移動する壁
const CVector SWITCH_MOVE_WALL_POS =			CVector(80.0f, 0.0f, 50.0f);
const CVector SWITCH_MOVE_WALL_SCALE =			CVector(1.0f, 4.0f, 1.0f);
const CVector SWITCH_MOVE_WALL_MOVE =			CVector(100.0f, 0.0f, 0.0f);
constexpr float SWITCH_MOVE_WALL_MOVE_TIME =	12.0f;
// シールド
const CVector SHIELD_SCALE =					CVector( 1.0f,  1.0f, 1.0f);
const CVector SHIELD_POS =						CVector(-40.0f, 0.0f, 10.0f);

// コンストラクタ
CTestField::CTestField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TestField");

	// オブジェクトを生成
	CreateFieldObjects();
	// コライダ―を生成
	CreateCol("TestField");
}

// デストラクタ
CTestField::~CTestField()
{
}

// フィールドのオブジェクトを生成
void CTestField::CreateFieldObjects()
{
	// 水を生成
	CWater* water = new CWater(WATER_POS, WATER_SCALE);

	// 重りを生成
	CWeight* weight = new CWeight(WEIGHT_POS);

	// 空中オブジェクトを生成
	CAirConnectObj* air = new CAirConnectObj(AIR_CONNECT_POS);

	// ポータルを生成
	CPortal* portal = new CPortal();
	// 位置調整
	portal->Position(PORTAL_POS);

	// クリスタル
	CSwitch* crystal1 = new CSwitch(CRYSTAL_POS1, false, ESwitchType::eBatteries);
	crystal1->SetOnOff(true);
	CSwitch* crystal2 = new CSwitch(CRYSTAL_POS2, false, ESwitchType::eBatteries);
	CSwitch* crystal3 = new CSwitch(CRYSTAL_POS3, false, ESwitchType::eBatteries);

	CSwitchMoveAirObj* switchAirObj =
		new CSwitchMoveAirObj(SWITCH_AIR_OBJ_POS, { SWITCH_AIR_OBJ_MOVE_POS1, SWITCH_AIR_OBJ_MOVE_POS2 });
	crystal1->SetActionObj(switchAirObj);

	// 押し出す壁
	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	CSwitchPushWall* switchPushWall =
		new CSwitchPushWall(model, model, SWITCH_PUSH_WALL_POS,
			SWITCH_PUSH_WALL_SCALE, SWITCH_PUSH_WALL_MOVE, SWITCH_PUSH_WALL_MOVE_TIME);
	crystal2->SetActionObj(switchPushWall);
	// スイッチで移動する壁
	CSwitchMoveWall* switchMoveWall =
		new CSwitchMoveWall(model, model, SWITCH_MOVE_WALL_POS,
			SWITCH_MOVE_WALL_SCALE, SWITCH_MOVE_WALL_MOVE, SWITCH_MOVE_WALL_MOVE_TIME);
	crystal2->SetActionObj(switchMoveWall);

	//// シールド
	//CSwitchShield* switchShield =
	//	new CSwitchShield(SHIELD_SCALE);
	//switchShield->Position(SHIELD_POS);
	//crystal3->SetActionObj(switchShield);

	CShield* shield = new CShield();
	shield->Position(SHIELD_POS);
}
