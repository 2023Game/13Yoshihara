#include "CCharaStatusBase.h"

// コンストラクタ
CCharaStatusBase::CCharaStatusBase(int maxHp, int maxMp, int regeneMp,
	float baseMoveSpeed, float jumpSpeed)
	: mMaxHp(maxHp)
	, mHp(mMaxHp)
	, mMaxMp(maxMp)
	, mMp(mMaxMp)
	, mRegeneMp(regeneMp)
	, mBaseMoveSpeed(baseMoveSpeed)
	, mJumpSpeed(jumpSpeed)
{
}

// デストラクタ
CCharaStatusBase::~CCharaStatusBase()
{
}

// ダメージを受ける
void CCharaStatusBase::TakeDamage(int damage, CObjectBase* attacker)
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

// MPを使用
bool CCharaStatusBase::UseMp(int useMp)
{
	// MPが足りないなら失敗
	if (mMp < useMp) return false;

	// 消費分を減らす
	mMp -= useMp;

	// 成功
	return true;
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

// 現在HPを加減算する
void CCharaStatusBase::SetHp(int num)
{
	mHp += num;
	// Hpが0以下なら0にする
	if (mHp <= 0)
	{
		mHp = 0;
	}
}

// 現在HPをリセット
void CCharaStatusBase::ResetHp()
{
	mHp = mMaxHp;
}

// 最大MPを取得
float CCharaStatusBase::GetMaxMp() const
{
	return mMaxMp;
}

// 最大MPを設定
void CCharaStatusBase::SetMaxMp(float maxMp)
{
	mMaxMp = maxMp;
}

// 現在MPを取得
float CCharaStatusBase::GetMp() const
{
	return mMp;
}

// 現在MPを加減算する
void CCharaStatusBase::SetMp(float num)
{
	mMp += num;
	// Mpが0以下なら0
	if (mMp <= 0)
	{
		mMp = 0;
	}
	// Mpが最大値より大きいなら最大値
	else if (mMp > mMaxMp)
	{
		mMp = mMaxMp;
	}
}

// 現在MPをリセット
void CCharaStatusBase::ResetMp()
{
	mMp = mMaxMp;
}

// MPの再生
void CCharaStatusBase::RegeneMp()
{
	SetMp(mRegeneMp * Times::DeltaTime());
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