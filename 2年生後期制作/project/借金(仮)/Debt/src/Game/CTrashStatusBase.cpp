#include "CTrashStatusBase.h"

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

// 全てのステータスを設定
void CTrashStatusBase::SetAllStatus(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockbackDealt, int attackPower, int power)
{
	SetMaxHp(maxHp);
	SetBaseMoveSpeed(baseMoveSpeed);
	SetJumpSpeed(jumpSpeed);
	SetKnockbackDealt(knockbackDealt);
	SetAttackPower(attackPower);
	SetPower(power);
}

// 与えるノックバック速度を取得
float CTrashStatusBase::GetKnockbackDealt() const
{
	return mKnockbackDealt;
}

// 与えるノックバック速度を設定
void CTrashStatusBase::SetKnockbackDealt(float knockbackDealt)
{
	mKnockbackDealt = knockbackDealt;
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
