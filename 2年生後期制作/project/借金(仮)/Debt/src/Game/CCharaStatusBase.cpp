#include "CCharaStatusBase.h"

// コンストラクタ
CCharaStatusBase::CCharaStatusBase(int maxHp, float baseMoveSpeed,
	float jumpSpeed, int attackPower)
	: mMaxHp(maxHp)
	, mHp(mMaxHp)
	, mBaseMoveSpeed(baseMoveSpeed)
	, mJumpSpeed(jumpSpeed)
	, mAttackPower(attackPower)
{
}

// デストラクタ
CCharaStatusBase::~CCharaStatusBase()
{
}

// ダメージを受ける
void CCharaStatusBase::TakeDamage(int damage, CObjectBase* causer)
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
void CCharaStatusBase::Death()
{
}

// 死んでいるかどうか
bool CCharaStatusBase::IsDeath() const
{
	// 現在HPが0なら死亡
	return mHp <= 0;
}

// 最大HPを取得
int CCharaStatusBase::GetMaxHp() const
{
	return mMaxHp;
}

// 最大HPを設定
void CCharaStatusBase::SetMaxHp(int maxHp)
{
	mMaxHp = maxHp;
}

// 現在HPを取得
int CCharaStatusBase::GetHp() const
{
	return mHp;
}

// 基礎移動速度を取得
float CCharaStatusBase::GetBaseMoveSpeed() const
{
	return mBaseMoveSpeed;
}

// 基礎移動速度を設定
void CCharaStatusBase::SetBaseMoveSpeed(float baseMoveSpeed)
{
	mBaseMoveSpeed = baseMoveSpeed;
}

// ジャンプ速度を取得
float CCharaStatusBase::GetJumpSpeed() const
{
	return mJumpSpeed;
}

// ジャンプ速度を設定
void CCharaStatusBase::SetJumpSpeed(float jumpSpeed)
{
	mJumpSpeed = jumpSpeed;
}

// 攻撃力を取得
int CCharaStatusBase::GetAttackPower() const
{
	return mAttackPower;
}

// 攻撃力を設定
void CCharaStatusBase::SetAttackPower(int attackPower)
{
	mAttackPower = attackPower;
}
