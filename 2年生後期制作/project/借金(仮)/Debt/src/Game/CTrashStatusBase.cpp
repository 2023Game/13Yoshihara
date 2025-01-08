#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockbackDealt, int attackPower, int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed, attackPower)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(CVector::zero)
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

// 受けるノックバックを取得
CVector CTrashStatusBase::GetKnockbackReceived() const
{
	return mKnockbackReceived;
}

// 受けるノックバックを設定
void CTrashStatusBase::SetKnockbackReceived(CVector knockbackReceived)
{
	mKnockbackReceived = knockbackReceived;
}