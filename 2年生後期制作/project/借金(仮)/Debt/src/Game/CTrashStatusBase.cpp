#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockback,
	int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed)
	, mKnockback(knockback)
	, mPower(power)
	, mTakeKnockback(0)
{
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// 与えるノックバック速度を取得
float CTrashStatusBase::GetKnockback() const
{
	return mKnockback;
}

// ゴミを落とす力を取得
int CTrashStatusBase::GetPower() const
{
	return mPower;
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