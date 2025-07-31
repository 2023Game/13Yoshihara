#pragma once
#include "CRoomBase.h"

class CBox;
class CMoveObj;
class CSwitch;
class CSwitchMoveWall;
class CWater;
class CRespawnArea;

// 部屋5
class CRoom5 : public CRoomBase
{
public:
	// コンストラクタ
	CRoom5(const CVector& pos);
	// デストラクタ
	~CRoom5();

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// フィールドオブジェクトを削除
	void DeleteFieldObjects() override;

	// オブジェクト
	CBox* mpBox1;
	CBox* mpBox2;
	CMoveObj* mpMoveFloor;
	CSwitch* mpSwitch;
	CSwitchMoveWall* mpMoveWall;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};