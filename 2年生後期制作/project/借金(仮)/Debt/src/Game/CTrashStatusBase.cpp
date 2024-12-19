#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase(int maxHp,float moveSpeed,float jumpSpeed,float knockback,
	int power)
	: mMaxHp(maxHp)
	, mHp(mMaxHp)
	, mMoveSpeed(moveSpeed)
	, mJumpSpeed(jumpSpeed)
	, mKnockback(knockback)
	, mPower(power)
	, mTakeKnockback(0)
{
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// ダメージを受ける
void CTrashStatusBase::TakeDamage(int damage, CObjectBase* causer)
{
	// 既に死亡していたら、ダメージを受けない
	if (IsDeath()) return;

	// 受けたダメージが現在HP以上なら
	if (damage >= mHp)
	{
		// HPを0にして、死亡
		mHp = 0;
		Death();
	}
	// 現在HPの方が多い場合は、ダメージ分減らす
	else
	{
		mHp -= damage;
	}
}

// 死亡
void CTrashStatusBase::Death()
{
}

// 死んでいるかどうか
bool CTrashStatusBase::IsDeath() const
{
	// 現在HPが0ならば、死亡
	return mHp <= 0;
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
	return mMoveSpeed;
}
// 基礎移動速度を設定
void CTrashStatusBase::SetBaseMoveSpeed(float baseMoveSpeed)
{
	mMoveSpeed = baseMoveSpeed;
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