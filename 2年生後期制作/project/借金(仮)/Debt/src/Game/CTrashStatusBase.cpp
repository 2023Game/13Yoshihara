#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockbackDealt,
	int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(0)
{
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// 与えるノックバック速度を取得
float CTrashStatusBase::GetKnockbackDealt() const
{
	return mKnockbackDealt;
}

// ゴミを落とす力を取得
int CTrashStatusBase::GetPower() const
{
	return mPower;
}

// 受けるノックバック速度を取得
float CTrashStatusBase::GetKnockbackReceived() const
{
	return mKnockbackReceived;
}

// 受けるノックバック速度を設定
void CTrashStatusBase::SetKnockbackReceived(float knockbackReceived)
{
	mKnockbackReceived = knockbackReceived;
}