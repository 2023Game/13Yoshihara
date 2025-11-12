#pragma once
#include "CRoomBase.h"

class CSwitch;
class CSwitchMoveAirObj;
class CWater;
class CRespawnArea;

class CRoom6 : public CRoomBase
{
public:
	CRoom6(const CVector& pos);
	~CRoom6();

	void SetEnableRoom(bool enable) override;

private:
	void CreateFieldObjects() override;

	// オブジェクト
	// TODO：重り生成器
	// TODO：バッテリー
	// TODO：シールド
	// TODO：押し出す壁
	CSwitch* mpSwitch1;
	CSwitch* mpSwitch2;
	CSwitch* mpCrystal1;
	CSwitch* mpCrystal2;
	CSwitch* mpCrystal3;
	CSwitch* mpCrystal4;
	CSwitchMoveAirObj* mpSwitchMoveAirObj1;
	CSwitchMoveAirObj* mpSwitchMoveAirObj2;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};