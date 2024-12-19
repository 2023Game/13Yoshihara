#include "CCharaStatusBase.h"

// コンストラクタ
CCharaStatusBase::CCharaStatusBase(int maxHp, float moveSpeed, float jumpSpeed)
	: mMaxHp(maxHp)
	, mHp(mMaxHp)
	, mMoveSpeed(moveSpeed)
	, mJumpSpeed(jumpSpeed)
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

// ジャンプ速度を取得
float CCharaStatusBase::GetJumpSpeed() const
{
	return mJumpSpeed;
}
