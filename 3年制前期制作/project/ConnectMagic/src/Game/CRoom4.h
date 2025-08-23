#pragma once
#include "CRoomBase.h"

class CBox;
class CMoveObj;
class CWater;
class CRespawnArea;

// 部屋4
class CRoom4 : public CRoomBase
{
public:
	// コンストラクタ
	CRoom4(const CVector& pos);
	// デストラクタ
	~CRoom4();

	// 部屋の有効無効を設定
	void SetEnableRoom(bool enable) override;

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;

	// オブジェクト
	CBox* mpBox;
	CMoveObj* mpMoveFloor;
	CMoveObj* mpMoveWall;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};