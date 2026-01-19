#pragma once
#include "CRoomBase.h"

class CWeight;
class CSwitch;
class CSwitchMoveFloor;
class CMoveObj;
class CWater;

// 部屋3
class CRoom3 : public CRoomBase
{
public:
	// コンストラクタ
	CRoom3(const CVector& pos);
	// デストラクタ
	~CRoom3();

	// 部屋の有効無効を設定
	void SetEnableRoom(bool enable) override;

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;

	// オブジェクト
	CWeight* mpWeight1;
	CWeight* mpWeight2;
	CSwitch* mpSwitch1;
	CSwitch* mpSwitch2;
	CSwitchMoveFloor* mpSwitchMoveFloor1;
	CSwitchMoveFloor* mpSwitchMoveFloor2;
	CMoveObj* mpMoveFloor1;
	CMoveObj* mpMoveFloor2;
	CMoveObj* mpMoveFloor3;
	CWater* mpWater;
};