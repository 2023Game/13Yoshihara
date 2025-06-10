#pragma once
#include "CObjectBase.h"

/// <summary>
/// キャラクターのベースクラス
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	virtual ~CCharaBase();

	// 更新
	void Update() override;

	// 移動方向を取得する
	CVector GetMoveDir();

protected:
	CVector mMoveSpeed;	// 前後左右の移動速度
	float mMoveSpeedY;	// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;	// 接地しているかどうか
	bool mIsGravity;	// 重力を掛けるかどうか
	bool mIsMoveDir;	// 移動方向を向くかどうか

	CVector mGroundNormal;	// 接地している地面の法線

	// 移動方向
	CVector mMoveDir;

	CTransform* mpRideObject;
};
