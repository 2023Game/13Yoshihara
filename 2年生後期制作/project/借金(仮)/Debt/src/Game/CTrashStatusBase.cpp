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

// �_���[�W��1�󂯂�
void CTrashStatusBase::TakeDamage()
{
	mStatus.mHp--;
}

// �󂯂�m�b�N�o�b�N���x��ݒ�
void CTrashStatusBase::SetTakeKnockBack(float knockback)
{
	mTakeKnockback = knockback;
}
