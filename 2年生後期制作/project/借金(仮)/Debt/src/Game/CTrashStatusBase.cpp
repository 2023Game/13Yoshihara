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

// 受けるノックバック速度を設定
void CTrashStatusBase::SetTakeKnockBack(float knockback)
{
	mTakeKnockback = knockback;
}
