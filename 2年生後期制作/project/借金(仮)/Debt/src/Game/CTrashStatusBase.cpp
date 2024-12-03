#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase()
	: mTakeKnockback(0)
{
	mStatus =
	{
		0,
		0.0f,
		0.0f,
		0,
	};
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// ダメージを1受ける
void CTrashStatusBase::TakeDamage()
{
	mStatus.mHp--;
}

// ノックバックの値を取得
float CTrashStatusBase::GetTakeKnockback()
{
	return mTakeKnockback;
}

// 受けるノックバック速度を設定
void CTrashStatusBase::SetTakeKnockback(float knockback)
{
	mTakeKnockback = knockback;
}

// ベースステータスを取得する
CTrashStatusBase::StatusBase CTrashStatusBase::GetStatusBase()
{
	return mStatus;
}

// ベースステータスを設定する
void CTrashStatusBase::SetStatusBase(int hp, float moveSpeed, float jumpSpeed, float knockback, int power)
{
	mStatus = 
	{
		hp,
		moveSpeed,
		jumpSpeed,
		knockback,
		power,
	};
}
