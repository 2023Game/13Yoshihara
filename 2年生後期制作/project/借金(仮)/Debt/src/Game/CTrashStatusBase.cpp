#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase(int maxHp,float moveSpeed,float jumpSpeed,float knockback,
	int power)
	: mMaxHp(maxHp)
	, mHp(mMaxHp)
	, mMoveSpeed(moveSpeed)
	, mJumpSpeed(jumpSpeed)
	, mKnockback(knockback)
	, mPower(power)
	, mTakeKnockback(0)
{
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// �_���[�W���󂯂�
void CTrashStatusBase::TakeDamage(int damage, CObjectBase* causer)
{
	// ���Ɏ��S���Ă�����A�_���[�W���󂯂Ȃ�
	if (IsDeath()) return;

	// �󂯂��_���[�W������HP�ȏ�Ȃ�
	if (damage >= mHp)
	{
		// HP��0�ɂ��āA���S
		mHp = 0;
		Death();
	}
	// ����HP�̕��������ꍇ�́A�_���[�W�����炷
	else
	{
		mHp -= damage;
	}
}

// ���S
void CTrashStatusBase::Death()
{
}

// ����ł��邩�ǂ���
bool CTrashStatusBase::IsDeath() const
{
	// ����HP��0�Ȃ�΁A���S
	return mHp <= 0;
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
	return mMoveSpeed;
}
// ��b�ړ����x��ݒ�
void CTrashStatusBase::SetBaseMoveSpeed(float baseMoveSpeed)
{
	mMoveSpeed = baseMoveSpeed;
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