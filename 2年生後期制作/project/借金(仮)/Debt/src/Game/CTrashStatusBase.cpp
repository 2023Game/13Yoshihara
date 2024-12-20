#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockback,
	int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed)
	, mKnockback(knockback)
	, mPower(power)
	, mTakeKnockback(0)
{
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// �^����m�b�N�o�b�N���x���擾
float CTrashStatusBase::GetKnockback() const
{
	return mKnockback;
}

// �S�~�𗎂Ƃ��͂��擾
int CTrashStatusBase::GetPower() const
{
	return mPower;
}

// �󂯂�m�b�N�o�b�N���x���擾
float CTrashStatusBase::GetTakeKnockback() const
{
	return mTakeKnockback;
}

// �󂯂�m�b�N�o�b�N���x��ݒ�
void CTrashStatusBase::SetTakeKnockback(float knockback)
{
	mTakeKnockback = knockback;
}