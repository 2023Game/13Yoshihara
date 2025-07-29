#pragma once
#include "CRoomBase.h"

class CPortal;

// 最後の部屋
class CLastRoom : public CRoomBase
{
public:
	// コンストラクタ
	CLastRoom(const CVector& pos);
	// デストラクタ
	~CLastRoom();

private:
	// オブジェクトを生成
	void CreateFieldObjects() override;
	
	// ポータル
	CPortal* mpPortal;
};