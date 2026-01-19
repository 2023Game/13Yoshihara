#pragma once
#include "CRoomBase.h"

class CWeight;
class CSwitch;
class CSwitchMoveFloor;
class CWater;

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
	CWeight* mpWeight;
	CSwitch* mpSwitch;
	CSwitchMoveFloor* mpMoveFloor;
	CWater* mpWater;
};