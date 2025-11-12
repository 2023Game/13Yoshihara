#pragma once
#include "CRoomBase.h"

class CWeight;
class CMoveObj;
class CWater;
class CRespawnArea;

// 部屋2
class CRoom2 : public CRoomBase
{
public:
	// コンストラクタ
	CRoom2(const CVector& pos);
	// デストラクタ
	~CRoom2();

	// 部屋の有効無効を設定
	void SetEnableRoom(bool enable) override;

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;

	// オブジェクト
	CWeight* mpWeight;
	CMoveObj* mpMoveFloor;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};