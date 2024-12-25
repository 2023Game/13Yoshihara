#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockbackDealt,
	int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(0)
{
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// �^����m�b�N�o�b�N���x���擾
float CTrashStatusBase::GetKnockbackDealt() const
{
	return mKnockbackDealt;
}

// �S�~�𗎂Ƃ��͂��擾
int CTrashStatusBase::GetPower() const
{
	return mPower;
}

// �󂯂�m�b�N�o�b�N���x���擾
float CTrashStatusBase::GetKnockbackReceived() const
{
	return mKnockbackReceived;
}

// �󂯂�m�b�N�o�b�N���x��ݒ�
void CTrashStatusBase::SetKnockbackReceived(float knockbackReceived)
{
	mKnockbackReceived = knockbackReceived;
}