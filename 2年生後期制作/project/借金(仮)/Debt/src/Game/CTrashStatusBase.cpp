#include "CTrashStatusBase.h"

// �R���X�g���N�^
CTrashStatusBase::CTrashStatusBase()
	: CCharaStatusBase(0, 0.0f, 0.0f, 0)
	, mKnockbackDealt(0.0f)
	, mPower(0)
	, mKnockbackReceived(CVector::zero)
{
}

// �R���X�g���N�^
CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockbackDealt, int attackPower, int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed, attackPower)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(CVector::zero)
{
}

// �f�X�g���N�^
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