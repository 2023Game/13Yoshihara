#include "CRoom6.h"
#include "CSwitch.h"
#include "CCrystal.h"
#include "CSwitchMoveAirObj.h"
#include "CAirConnectObj.h"
#include "CWater.h"
#include "CRespawnArea.h"

#define ROOM_LENGTH					200.0f

#define SWITCH_OFFSET_POS1			CVector(-30.0f,		 0.0f,		-100.0f)
#define SWITCH_OFFSET_POS2			CVector( 30.0f,		 0.0f,		-100.0f)

#define AIR_OBJ1_MOVE_TARGET_POS1	CVector(-30.0f,		 35.0f,		-25.0f)
#define AIR_OBJ1_MOVE_TARGET_POS2	CVector(-145.0f,	 35.0f,		-25.0f)

#define AIR_OBJ2_MOVE_TARGET_POS1	CVector( 30.0f,		 35.0f,		-25.0f)
#define AIR_OBJ2_MOVE_TARGET_POS2	CVector( 145.0f,	 35.0f,		-25.0f)

#define CRYSTAL_OFFSET_POS1			CVector(-30.0f,		 0.0f,		-35.0f)
#define CRYSTAL_OFFSET_POS2			CVector( 30.0f,		 0.0f,		-35.0f)
#define CRYSTAL_OFFSET_POS3			CVector(-67.5f,		 40.0f,		-35.0f)
#define CRYSTAL_OFFSET_POS4			CVector( 67.5f,		 40.0f,		-35.0f)

#define ROT_X_180					CVector( 180.0f,	 0.0f,		 0.0f)
#define ROT_Y_90					CVector( 0.0f,		 90.0f,		 0.0f)

#define WATER_OFFSET_POS			CVector( 0.0f,		-15.0f,		-100.0f)
#define WATER_SCALE					CVector( 200.0f,	 0.0f,		 100.0f)

#define RESPAWN_OFFSET_POS			CVector( 0.0f,		 0.0f,		-30.0f)
#define RESPAWN_RADIUS				30.0f

CRoom6::CRoom6(const CVector& pos)
	: CRoomBase(ROOM_LENGTH)
{
	mpModel = CResourceManager::Get<CModel>("Room6");
	Position(pos);

	CreateCol("Room6_Ground_Col", "Room6_Wall_Col", "");

	CreateFieldObjects();
}

CRoom6::~CRoom6()
{
}

void CRoom6::SetEnableRoom(bool enable)
{
	CRoomBase::SetEnableRoom(enable);
	mpSwitch1->SetEnableSwitch(enable);
	mpSwitch2->SetEnableSwitch(enable);
	mpCrystal1->SetEnableSwitch(enable);
	mpCrystal2->SetEnableSwitch(enable);
	mpCrystal3->SetEnableSwitch(enable);
	mpCrystal4->SetEnableSwitch(enable);
}

void CRoom6::CreateFieldObjects()
{
	mpSwitch1 = new CSwitch(Position() + SWITCH_OFFSET_POS1, true);
	mpSwitch2 = new CSwitch(Position() + SWITCH_OFFSET_POS2, true);

	// new CSwitchMoveAirObj(初期座標,移動先のリスト)
	mpSwitchMoveAirObj1 = new CSwitchMoveAirObj(Position() + AIR_OBJ1_MOVE_TARGET_POS1,
		{ Position() + AIR_OBJ1_MOVE_TARGET_POS1,
		  Position() + AIR_OBJ1_MOVE_TARGET_POS2 });
	mpSwitchMoveAirObj2 = new CSwitchMoveAirObj(Position() + AIR_OBJ2_MOVE_TARGET_POS1,
		{ Position() + AIR_OBJ2_MOVE_TARGET_POS1, 
		  Position() + AIR_OBJ2_MOVE_TARGET_POS2 });
	mObjs.push_back(mpSwitchMoveAirObj1);
	mObjs.push_back(mpSwitchMoveAirObj2);
	// 方向調整
	mpSwitchMoveAirObj1->GetAirConnectObj()->Rotate(-ROT_Y_90);
	mpSwitchMoveAirObj2->GetAirConnectObj()->Rotate(-ROT_Y_90);

	mpCrystal1 = new CSwitch(Position() + CRYSTAL_OFFSET_POS1, false, ESwitchType::eBatteries);
	mpCrystal2 = new CSwitch(Position() + CRYSTAL_OFFSET_POS2, false, ESwitchType::eBatteries);
	mpCrystal3 = new CSwitch(Position() + CRYSTAL_OFFSET_POS3, false, ESwitchType::eBatteries);
	mpCrystal4 = new CSwitch(Position() + CRYSTAL_OFFSET_POS4, false, ESwitchType::eBatteries);
	//上下反転
	mpCrystal3->GetCrystal()->Rotate(ROT_X_180);
	mpCrystal4->GetCrystal()->Rotate(ROT_X_180);
	// 作用するオブジェクトを設定
	mpCrystal3->SetActionObj(mpSwitchMoveAirObj1);
	mpCrystal4->SetActionObj(mpSwitchMoveAirObj2);
	// クリスタルをオン状態へ
	mpCrystal1->SetOnOff(true);
	mpCrystal2->SetOnOff(true);

	mpWater = new CWater(WATER_SCALE);
	mpWater->Position(Position() + WATER_OFFSET_POS);
	mObjs.push_back(mpWater);

	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
	mObjs.push_back(mpRespawnArea);
}
