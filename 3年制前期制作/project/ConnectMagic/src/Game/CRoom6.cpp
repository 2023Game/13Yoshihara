#include "CRoom6.h"
#include "CSwitch.h"
#include "CCrystal.h"
#include "CSwitchMoveAirObj.h"
#include "CAirConnectObj.h"
#include "CSwitchPushWall.h"
#include "CWater.h"
#include "CRespawnArea.h"
#include "CSaveManager.h"
#include "CSwitchShield.h"
#include "CWeightGenerator.h"

#define ROOM_LENGTH					200.0f

#define SWITCH_OFFSET_POS1			CVector(-30.0f,		 0.0f,		-100.0f)
#define SWITCH_OFFSET_POS2			CVector( 30.0f,		 0.0f,		-100.0f)

#define AIR_OBJ1_MOVE_TARGET_POS1	CVector(-67.5f,		 35.0f,		-25.0f)
#define AIR_OBJ1_MOVE_TARGET_POS2	CVector(-145.0f,	 35.0f,		-25.0f)

#define AIR_OBJ2_MOVE_TARGET_POS1	CVector( 67.5f,		 35.0f,		-25.0f)
#define AIR_OBJ2_MOVE_TARGET_POS2	CVector( 145.0f,	 35.0f,		-25.0f)

#define SHIELD_POS1					CVector(-175.0f,	 0.0f,		-60.0f)
#define SHIELD_POS2					CVector(-175.0f,	 0.0f,		-90.0f)
#define SHIELD_POS3					CVector(-150.0f,	 0.0f,		-75.0f)
#define SHIELD_POS4					CVector(-175.0f,	 0.0f,		-110.0f)
#define SHIELD_POS5					CVector(-175.0f,	 0.0f,		-140.0f)
#define SHIELD_POS6					CVector(-150.0f,	 0.0f,		-125.0f)
#define SHIELD_SCALE_BIG			CVector( 2.5f,		 1.0f,		 1.0f)
#define SHIELD_SCALE_SMALL			CVector( 1.5f,		 1.0f,		 1.0f)

#define PUSH_WALL_POS1				CVector( 224.0f,	 0.0f,		-59.5f)
#define PUSH_WALL_POS2				CVector( 224.0f,	 0.0f,		-109.5f)
#define PUSH_WALL_SCALE				CVector( 5.1f,		 4.0f,		 3.1f)
#define PUSH_WALL_MOVE_VEC			CVector(-50.0f,		 0.0f,		 0.0f)
#define PUSH_WALL_MOVE_TIME			1.0f

#define GENERATOR_POS1				CVector(-30.0f,		 20.0f,		-100.0f)
#define GENERATOR_POS2				CVector( 30.0f,		 20.0f,		-100.0f)

#define CRYSTAL_OFFSET_POS1			CVector(-30.0f,		 0.0f,		-35.0f)
#define CRYSTAL_OFFSET_POS2			CVector( 30.0f,		 0.0f,		-35.0f)
#define CRYSTAL_OFFSET_POS3			CVector(-67.5f,		 36.0f,		-30.0f)
#define CRYSTAL_OFFSET_POS4			CVector( 67.5f,		 36.0f,		-30.0f)
#define CRYSTAL_OFFSET_POS5			CVector(-140.0f,	 40.0f,		-75.0f)
#define CRYSTAL_OFFSET_POS6			CVector(-140.0f,	 40.0f,		-125.0f)
#define CRYSTAL_OFFSET_POS7			CVector(-175.0f,	 0.0f,		-175.0f)
#define CRYSTAL_OFFSET_POS8			CVector( 140.0f,	 40.0f,		-75.0f)
#define CRYSTAL_OFFSET_POS9			CVector( 140.0f,	 40.0f,		-125.0f)
#define CRYSTAL_OFFSET_POS10		CVector( 175.0f,	 0.0f,		-175.0f)
#define CRYSTAL_SCALE_BIG			CVector::one * 2.0f

#define ROT_X_180					CVector( 180.0f,	 0.0f,		 0.0f)
#define ROT_X_90					CVector( 90.0f,		 0.0f,		 0.0f)
#define ROT_Y_180					CVector( 0.0f,		 180.0f,	 0.0f)
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
	mpCrystal5->SetEnableSwitch(enable);
	mpCrystal6->SetEnableSwitch(enable);
	mpCrystal7->SetEnableSwitch(enable);
	mpCrystal8->SetEnableSwitch(enable);
	mpCrystal9->SetEnableSwitch(enable);
	mpCrystal10->SetEnableSwitch(enable);

	// 保存管理クラス
	CSaveManager* saveMgr = CSaveManager::Instance();
	// 有効時のみ
	if (enable) {
		saveMgr->AddMoveAirObj(mpSwitchMoveAirObj1);
		saveMgr->AddMoveAirObj(mpSwitchMoveAirObj2);
		saveMgr->AddSwitch(mpCrystal1);
		saveMgr->AddSwitch(mpCrystal2);
		saveMgr->AddSwitch(mpCrystal3);
		saveMgr->AddSwitch(mpCrystal4);
		saveMgr->AddSwitch(mpCrystal5);
		saveMgr->AddSwitch(mpCrystal6);
		saveMgr->AddSwitch(mpCrystal7);
		saveMgr->AddSwitch(mpCrystal8);
		saveMgr->AddSwitch(mpCrystal9);
		saveMgr->AddSwitch(mpCrystal10);
	}
	// 無効時
	else
	{
		saveMgr->DeleteMoveAirObj(mpSwitchMoveAirObj1);
		saveMgr->DeleteMoveAirObj(mpSwitchMoveAirObj2);
		saveMgr->DeleteSwitch(mpCrystal1);
		saveMgr->DeleteSwitch(mpCrystal2);
		saveMgr->DeleteSwitch(mpCrystal3);
		saveMgr->DeleteSwitch(mpCrystal4);
		saveMgr->DeleteSwitch(mpCrystal5);
		saveMgr->DeleteSwitch(mpCrystal6);
		saveMgr->DeleteSwitch(mpCrystal7);
		saveMgr->DeleteSwitch(mpCrystal8);
		saveMgr->DeleteSwitch(mpCrystal9);
		saveMgr->DeleteSwitch(mpCrystal10);
	}
}

void CRoom6::CreateFieldObjects()
{
	mpSwitch1 = new CSwitch(Position() + SWITCH_OFFSET_POS1, true);
	mpSwitch2 = new CSwitch(Position() + SWITCH_OFFSET_POS2, true);

	mpCrystal1 = new CSwitch(Position() + CRYSTAL_OFFSET_POS1, false, ESwitchType::eBatteries);
	mpCrystal2 = new CSwitch(Position() + CRYSTAL_OFFSET_POS2, false, ESwitchType::eBatteries);
	// クリスタルをオン状態へ
	mpCrystal1->SetOnOff(true);
	mpCrystal2->SetOnOff(true);

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
	mpSwitchMoveAirObj1->GetAirConnectObj()->Rotate(ROT_Y_90);
	mpSwitchMoveAirObj2->GetAirConnectObj()->Rotate(ROT_Y_90);

	mpCrystal3 = new CSwitch(Position() + CRYSTAL_OFFSET_POS3, false, ESwitchType::eBatteries);
	mpCrystal4 = new CSwitch(Position() + CRYSTAL_OFFSET_POS4, false, ESwitchType::eBatteries);
	//方向調整
	mpCrystal3->GetCrystal()->Rotate(ROT_X_90);
	mpCrystal4->GetCrystal()->Rotate(ROT_X_90);
	// 親子設定で移動オブジェクトについていく
	mpCrystal3->GetCrystal()->SetParent(mpSwitchMoveAirObj1);
	mpCrystal4->GetCrystal()->SetParent(mpSwitchMoveAirObj2);
	// 作用するオブジェクトを設定
	mpCrystal3->SetActionObj(mpSwitchMoveAirObj1);
	mpCrystal4->SetActionObj(mpSwitchMoveAirObj2);

	// シールドは一度オフになるとそのまま
	mpShield1 = new CSwitchShield(SHIELD_SCALE_BIG);
	mpShield1->Position(Position() + SHIELD_POS1);
	mpShield2 = new CSwitchShield(SHIELD_SCALE_BIG);
	mpShield2->Position(Position() + SHIELD_POS2);
	mpShield3 = new CSwitchShield(SHIELD_SCALE_SMALL);
	mpShield3->Position(Position() + SHIELD_POS3);
	mpShield3->Rotate(ROT_Y_90);
	mpShield4 = new CSwitchShield(SHIELD_SCALE_BIG);
	mpShield4->Position(Position() + SHIELD_POS4);
	mpShield5 = new CSwitchShield(SHIELD_SCALE_BIG);
	mpShield5->Position(Position() + SHIELD_POS5);
	mpShield6 = new CSwitchShield(SHIELD_SCALE_SMALL);
	mpShield6->Position(Position() + SHIELD_POS6);
	mpShield6->Rotate(ROT_Y_90);
	mObjs.push_back(mpShield1);
	mObjs.push_back(mpShield2);
	mObjs.push_back(mpShield3);
	mObjs.push_back(mpShield4);
	mObjs.push_back(mpShield5);
	mObjs.push_back(mpShield6);

	mpCrystal5 = new CSwitch(Position() + CRYSTAL_OFFSET_POS5, false, ESwitchType::eBatteries);
	mpCrystal6 = new CSwitch(Position() + CRYSTAL_OFFSET_POS6, false, ESwitchType::eBatteries);
	// 方向調整
	mpCrystal5->GetCrystal()->Rotate(ROT_X_180);
	mpCrystal6->GetCrystal()->Rotate(ROT_X_180);
	// 作用するオブジェクトを設定
	mpCrystal5->SetActionObj(mpShield1);
	mpCrystal5->SetActionObj(mpShield2);
	mpCrystal5->SetActionObj(mpShield3);
	mpCrystal6->SetActionObj(mpShield4);
	mpCrystal6->SetActionObj(mpShield5);
	mpCrystal6->SetActionObj(mpShield6);

	CModel* model = CResourceManager::Get<CModel>("MoveObject");
	mpPushWall1 = new CSwitchPushWall(model, model,
		Position() + PUSH_WALL_POS1, PUSH_WALL_SCALE, 
		PUSH_WALL_MOVE_VEC, PUSH_WALL_MOVE_TIME);
	mpPushWall2 = new CSwitchPushWall(model, model,
		Position() + PUSH_WALL_POS2, PUSH_WALL_SCALE, 
		PUSH_WALL_MOVE_VEC, PUSH_WALL_MOVE_TIME);
	mObjs.push_back(mpPushWall1);
	mObjs.push_back(mpPushWall2);

	mpCrystal8 = new CSwitch(Position() + CRYSTAL_OFFSET_POS8, false, ESwitchType::eBatteries);
	mpCrystal9 = new CSwitch(Position() + CRYSTAL_OFFSET_POS9, false, ESwitchType::eBatteries);
	// 方向調整
	mpCrystal8->GetCrystal()->Rotate(ROT_X_180);
	mpCrystal9->GetCrystal()->Rotate(ROT_X_180);
	// 作用するオブジェクトを設定
	mpCrystal8->SetActionObj(mpPushWall1);
	mpCrystal9->SetActionObj(mpPushWall2);

	// 重り生成器
	mpGenerator1 = new CWeightGenerator(Position() + GENERATOR_POS1);
	mpGenerator2 = new CWeightGenerator(Position() + GENERATOR_POS2);
	mpGenerator2->Rotate(ROT_Y_180);
	mObjs.push_back(mpGenerator1);
	mObjs.push_back(mpGenerator2);
	// 大きいクリスタル
	mpCrystal7 = new CSwitch(Position() + CRYSTAL_OFFSET_POS7, false, ESwitchType::eBatteries);
	mpCrystal7->GetCrystal()->Scale(CRYSTAL_SCALE_BIG);
	mpCrystal10 = new CSwitch(Position() + CRYSTAL_OFFSET_POS10, false, ESwitchType::eBatteries);
	mpCrystal10->GetCrystal()->Scale(CRYSTAL_SCALE_BIG);
	mpCrystal7->SetActionObj(mpGenerator1);
	mpCrystal10->SetActionObj(mpGenerator2);
	mpCrystal7->SetOnOff(true);
	mpCrystal10->SetOnOff(true);

	mpWater = new CWater(WATER_SCALE);
	mpWater->Position(Position() + WATER_OFFSET_POS);
	mObjs.push_back(mpWater);

	mpRespawnArea = new CRespawnArea(Position() + RESPAWN_OFFSET_POS, RESPAWN_RADIUS);
	mObjs.push_back(mpRespawnArea);
}
