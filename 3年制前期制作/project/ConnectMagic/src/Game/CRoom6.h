#pragma once
#include "CRoomBase.h"

class CSwitch;
class CSwitchMoveAirObj;
class CSwitchShield;
class CSwitchPushWall;
class CSwitchWater;
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
	std::vector<CWeightGenerator*> mGenerators;
	std::vector<CSwitchShield*> mShields;
	std::vector<CSwitchPushWall*> mPushWalls;
	std::vector<CSwitchMoveAirObj*> mAirObjs;
	CSwitchWater* mpWater;
	std::vector<CSwitch*> mSwitches;
	std::vector<CSwitch*> mCrystals;
};