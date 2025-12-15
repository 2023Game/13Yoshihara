#pragma once
#include "CRoomBase.h"

class CSwitch;
class CSwitchMoveAirObj;
class CSwitchShield;
class CSwitchPushWall;
class CSwitchWater;
class CRespawnArea;
class CWeightGenerator;
class CWeight;

// 繋げる要素のチュートリアル的部屋
class CRoom6 : public CRoomBase
{
public:
	CRoom6(const CVector& pos);
	~CRoom6();

	void SetEnableRoom(bool enable) override;

private:
	void CreateFieldObjects() override;

	// オブジェクト
	CWeight* mpWeight;
	CWeightGenerator* mpGenerator1;
	CWeightGenerator* mpGenerator2;
	CSwitchShield* mpShield1;
	CSwitchShield* mpShield2;
	CSwitchShield* mpShield3;
	CSwitchShield* mpShield4;
	CSwitchShield* mpShield5;
	CSwitchShield* mpShield6;
	CSwitchPushWall* mpPushWall1;
	CSwitchPushWall* mpPushWall2;
	CSwitch* mpSwitch1;
	CSwitch* mpSwitch2;
	CSwitch* mpCrystal1;
	CSwitch* mpCrystal2;
	CSwitch* mpCrystal3;
	CSwitch* mpCrystal4;
	CSwitch* mpCrystal5;
	CSwitch* mpCrystal6;
	CSwitch* mpCrystal7;
	CSwitch* mpCrystal8;
	CSwitch* mpCrystal9;
	CSwitch* mpCrystal10;
	CSwitchMoveAirObj* mpSwitchMoveAirObj1;
	CSwitchMoveAirObj* mpSwitchMoveAirObj2;
	CSwitchWater* mpWater;
	CRespawnArea* mpRespawnArea;
};