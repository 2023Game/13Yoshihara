#include "CCharaBase.h"

CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsGravity(true)
	, mIsMoveDir(true)
	, mpRideObject(nullptr)
{
}

CCharaBase::~CCharaBase()
{
}

// 更新
void CCharaBase::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 重力を掛けるなら
	if (mIsGravity)
	{
		//mMoveSpeedY -= GRAVITY;
	}
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	// 移動方向を設定
	mMoveDir = moveSpeed.Normalized();

	// 移動
	Position(Position() + moveSpeed);

	// 攻撃を受けていない時かつ
	// 移動方向を向く設定がオンの時
	if (!mIsDamage &&
		mIsMoveDir)
	{
		// プレイヤーを移動方向へ向ける
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));
	}
}

// 移動方向を取得する
CVector CCharaBase::GetMoveDir()
{
	return mMoveDir;
}
