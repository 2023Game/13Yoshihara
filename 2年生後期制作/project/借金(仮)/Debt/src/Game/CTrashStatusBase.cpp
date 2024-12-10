#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase()
	: mHp(0)
	, mBaseMoveSpeed(0.0f)
	, mJumpSpeed(0.0f)
	, mKnockback(0.0f)
	, mPower(0)
	, mTakeKnockback(0)
{
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// �_���[�W��1�󂯂�
void CTrashStatusBase::TakeDamage()
{
	SetHp(GetHp() - 1);
}

// ����HP���擾
int CTrashStatusBase::GetHp() const
{
	return mHp;
}
// HP��ݒ�
void CTrashStatusBase::SetHp(int hp)
{
	mHp = hp;
}

// �ړ����x���擾
float CTrashStatusBase::GetBaseMoveSpeed() const
{
	return mBaseMoveSpeed;
}
// ��b�ړ����x��ݒ�
void CTrashStatusBase::SetBaseMoveSpeed(float baseMoveSpeed)
{
	mBaseMoveSpeed = baseMoveSpeed;
}

// �W�����v���x���擾
float CTrashStatusBase::GetJumpSpeed() const
{
	return mJumpSpeed;
}
// �W�����v���x��ݒ�
void CTrashStatusBase::SetJumpSpeed(float jumpSpeed)
{
	mJumpSpeed = jumpSpeed;
}

// �^����m�b�N�o�b�N���x���擾
float CTrashStatusBase::GetKnockback() const
{
	return mKnockback;
}
// �^����m�b�N�o�b�N���x��ݒ�
void CTrashStatusBase::SetKnockback(float knockback)
{
	mKnockback = knockback;
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