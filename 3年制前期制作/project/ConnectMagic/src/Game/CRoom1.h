#pragma once
#include "CRoomBase.h"

class CBox;
class CSwitch;
class CSwitchMoveFloor;
class CWater;
class CRespawnArea;

// 部屋1
class CRoom1 : public CRoomBase
{
public:
	// コンストラクタ
	CRoom1(const CVector& pos);
	// デストラクタ
	~CRoom1();

	// 部屋の有効無効を設定
	void SetEnableRoom(bool enable) override;

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;

	// オブジェクト
	CBox* mpBox;
	CSwitch* mpSwitch;
	CSwitchMoveFloor* mpMoveFloor;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};