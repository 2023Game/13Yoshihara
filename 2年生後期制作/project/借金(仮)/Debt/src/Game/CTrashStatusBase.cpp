#include "CTrashStatusBase.h"

// コンストラクタ
CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockbackDealt, int attackPower, int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed, attackPower)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(CVector::zero)
	, mTrashBagNum(0)
	, mGoldTrashBagNum(0)
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

// ゴミ袋の数を加算する
void CTrashStatusBase::SetTrashBag(int num)
{
	mTrashBagNum += num;
	// 0より小さくなったら0にする
	if (mTrashBagNum < 0) mTrashBagNum = 0;
}

// ゴミ袋の数を取得する
int CTrashStatusBase::GetTrashBag() const
{
	return mTrashBagNum;
}

// ゴールドゴミ袋の数を加算する
void CTrashStatusBase::SetGoldTrashBag(int num)
{
	mGoldTrashBagNum += num;
	// 0より小さくなったら0にする
	if (mGoldTrashBagNum < 0) mGoldTrashBagNum = 0;
}

// ゴールドゴミ袋の数を取得する
int CTrashStatusBase::GetGoldTrashBag() const
{
	return mGoldTrashBagNum;
}
