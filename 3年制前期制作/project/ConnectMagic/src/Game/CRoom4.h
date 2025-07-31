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

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// フィールドオブジェクトを削除
	void DeleteFieldObjects() override;

	// オブジェクト
	CBox* mpBox;
	CMoveObj* mpMoveFloor;
	CMoveObj* mpMoveWall;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};