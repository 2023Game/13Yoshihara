#pragma once
#include "CRoomBase.h"

class CBox;
class CSwitch;
class CSwitchMoveWall;

// 部屋と部屋を繋げる部屋
class CConnectRoom : public CRoomBase
{
public:
	// コンストラクタ
	CConnectRoom(const CVector& pos);
	// デストラクタ
	~CConnectRoom();

	// 次の部屋を設定
	void SetNextRoom(CRoomBase* room);
	// 前の部屋を設定
	void SetPreRoom(CRoomBase* room);

private:
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// フィールドオブジェクトを削除
	void DeleteFieldObjects() override;

	// オブジェクト
	CBox* mpBox;
	CSwitch* mpNextSwitch;
	CSwitch* mpPreSwitch;
	CSwitchMoveWall* mpNextWall;
	CSwitchMoveWall* mpPreWall;

	// 次の部屋
	CRoomBase* mpNextRoom;
	// 前の部屋
	CRoomBase* mpPreRoom;
};