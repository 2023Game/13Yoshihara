#pragma once
#include "CRoomBase.h"

class CBox;
class CSwitch;
class CSwitchMoveFloor;
class CMoveObj;
class CWater;
class CRespawnArea;

// 部屋3
class CRoom3 : public CRoomBase
{
public:
	// コンストラクタ
	CRoom3(const CVector& pos);
	// デストラクタ
	~CRoom3();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// フィールドオブジェクトを削除
	void DeleteFieldObjects() override;

	// オブジェクト
	CBox* mpBox1;
	CBox* mpBox2;
	CSwitch* mpSwitch1;
	CSwitch* mpSwitch2;
	CSwitchMoveFloor* mpSwitchMoveFloor1;
	CSwitchMoveFloor* mpSwitchMoveFloor2;
	CMoveObj* mpMoveFloor1;
	CMoveObj* mpMoveFloor2;
	CMoveObj* mpMoveFloor3;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};