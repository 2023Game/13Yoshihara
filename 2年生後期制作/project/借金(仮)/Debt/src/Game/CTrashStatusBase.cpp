#include "CTrashStatusBase.h"

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

// �S�ẴX�e�[�^�X��ݒ�
void CTrashStatusBase::SetAllStatus(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockbackDealt, int attackPower, int power)
{
	SetMaxHp(maxHp);
	SetBaseMoveSpeed(baseMoveSpeed);
	SetJumpSpeed(jumpSpeed);
	SetKnockbackDealt(knockbackDealt);
	SetAttackPower(attackPower);
	SetPower(power);
}

// �^����m�b�N�o�b�N���x���擾
float CTrashStatusBase::GetKnockbackDealt() const
{
	return mKnockbackDealt;
}

// �^����m�b�N�o�b�N���x��ݒ�
void CTrashStatusBase::SetKnockbackDealt(float knockbackDealt)
{
	mKnockbackDealt = knockbackDealt;
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

// �S�~�𗎂Ƃ��͂��擾
int CTrashStatusBase::GetPower() const
{
	return mPower;
}

// �S�~�𗎂Ƃ��͂�ݒ�
void CTrashStatusBase::SetPower(int power)
{
	mPower = power;
}
