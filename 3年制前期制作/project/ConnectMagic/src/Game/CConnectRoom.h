#pragma once
#include "CRoomBase.h"

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

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// コライダーの生成
	void CreateCol() override;
	// フィールドオブジェクトを生成
	void CreateFieldObjects() override;
	// フィールドオブジェクトを削除
	void DeleteFieldObjects() override;

	// オブジェクト
	CSwitch* mpNextSwitch;
	CSwitchMoveWall* mpNextWall;
	CSwitchMoveWall* mpPreWall;

	// 次の部屋
	CRoomBase* mpNextRoom;
	// 前の部屋
	CRoomBase* mpPreRoom;

	// 前の部屋に繋がる扉が閉じるフラグ用のコライダー
	CCollider* mpCloseCol;
};