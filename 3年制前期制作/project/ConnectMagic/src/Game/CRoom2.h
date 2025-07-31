#pragma once
#include "CRoomBase.h"

class CBox;
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

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// フィールドオブジェクトを削除
	void DeleteFieldObjects() override;

	// オブジェクト
	CBox* mpBox;
	CMoveObj* mpMoveFloor;
	CWater* mpWater;
	CRespawnArea* mpRespawnArea;
};