#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockbackDealt, int attackPower, int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed, attackPower)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(CVector::zero)
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

// �󂯂�m�b�N�o�b�N���擾
CVector CTrashStatusBase::GetKnockbackReceived() const
{
	return mKnockbackReceived;
}

// �󂯂�m�b�N�o�b�N��ݒ�
void CTrashStatusBase::SetKnockbackReceived(CVector knockbackReceived)
{
	mKnockbackReceived = knockbackReceived;
}