#include "CTrashStatusBase.h"

// コンストラクタ
CTrashStatusBase::CTrashStatusBase()
	: CCharaStatusBase(0, 0.0f, 0.0f, 0)
	, mKnockbackDealt(0.0f)
	, mPower(0)
	, mKnockbackReceived(CVector::zero)
{
}

// コンストラクタ
CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockbackDealt, int attackPower, int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed, attackPower)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(CVector::zero)
{
}

// デストラクタ
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