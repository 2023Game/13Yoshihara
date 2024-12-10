#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase()
	: mHp(0)
	, mBaseMoveSpeed(0.0f)
	, mJumpSpeed(0.0f)
	, mKnockback(0.0f)
	, mPower(0)
	, mTakeKnockback(0)
{
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// ダメージを1受ける
void CTrashStatusBase::TakeDamage()
{
	SetHp(GetHp() - 1);
}

// 現在HPを取得
int CTrashStatusBase::GetHp() const
{
	return mHp;
}
// HPを設定
void CTrashStatusBase::SetHp(int hp)
{
	mHp = hp;
}

// 移動速度を取得
float CTrashStatusBase::GetBaseMoveSpeed() const
{
	return mBaseMoveSpeed;
}
// 基礎移動速度を設定
void CTrashStatusBase::SetBaseMoveSpeed(float baseMoveSpeed)
{
	mBaseMoveSpeed = baseMoveSpeed;
}

// ジャンプ速度を取得
float CTrashStatusBase::GetJumpSpeed() const
{
	return mJumpSpeed;
}
// ジャンプ速度を設定
void CTrashStatusBase::SetJumpSpeed(float jumpSpeed)
{
	mJumpSpeed = jumpSpeed;
}

// 与えるノックバック速度を取得
float CTrashStatusBase::GetKnockback() const
{
	return mKnockback;
}
// 与えるノックバック速度を設定
void CTrashStatusBase::SetKnockback(float knockback)
{
	mKnockback = knockback;
}

// ゴミを落とす力を取得
int CTrashStatusBase::GetPower() const
{
	return mPower;
}
// ゴミを落とす力を設定
void CTrashStatusBase::SetPower(int power)
{
	mPower = power;
}

// 受けるノックバック速度を取得
float CTrashStatusBase::GetTakeKnockback() const
{
	return mTakeKnockback;
}

// 受けるノックバック速度を設定
void CTrashStatusBase::SetTakeKnockback(float knockback)
{
	mTakeKnockback = knockback;
}