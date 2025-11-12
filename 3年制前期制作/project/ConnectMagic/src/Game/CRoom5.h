#pragma once
#include "CRoomBase.h"

class CWeight;
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

	// 部屋の有効無効を設定
	void SetEnableRoom(bool enable) override;

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;

	// オブジェクト
	CWeight* mpWeight1;
	CWeight* mpWeight2;
	CMoveObj* mpMoveFloor;
	CSwitch* mpSwitch;
	CSwitchMoveWall* mpMoveWall;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};