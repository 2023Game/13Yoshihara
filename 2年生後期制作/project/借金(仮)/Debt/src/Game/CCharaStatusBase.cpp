#include "CCharaStatusBase.h"

// �R���X�g���N�^
CCharaStatusBase::CCharaStatusBase(int maxHp, float baseMoveSpeed,
	float jumpSpeed, int attackPower)
	: mMaxHp(maxHp)
	, mHp(mMaxHp)
	, mBaseMoveSpeed(baseMoveSpeed)
	, mJumpSpeed(jumpSpeed)
	, mAttackPower(attackPower)
{
}

// �f�X�g���N�^
CCharaStatusBase::~CCharaStatusBase()
{
}

// �_���[�W���󂯂�
void CCharaStatusBase::TakeDamage(int damage, CObjectBase* causer)
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
void CCharaStatusBase::Death()
{
}

// ����ł��邩�ǂ���
bool CCharaStatusBase::IsDeath() const
{
	// ����HP��0�Ȃ玀�S
	return mHp <= 0;
}

// �ő�HP���擾
int CCharaStatusBase::GetMaxHp() const
{
	return mMaxHp;
}

// �ő�HP��ݒ�
void CCharaStatusBase::SetMaxHp(int maxHp)
{
	mMaxHp = maxHp;
}

// ����HP���擾
int CCharaStatusBase::GetHp() const
{
	return mHp;
}

// ��b�ړ����x���擾
float CCharaStatusBase::GetBaseMoveSpeed() const
{
	return mBaseMoveSpeed;
}

// ��b�ړ����x��ݒ�
void CCharaStatusBase::SetBaseMoveSpeed(float baseMoveSpeed)
{
	mBaseMoveSpeed = baseMoveSpeed;
}

// �W�����v���x���擾
float CCharaStatusBase::GetJumpSpeed() const
{
	return mJumpSpeed;
}

// �W�����v���x��ݒ�
void CCharaStatusBase::SetJumpSpeed(float jumpSpeed)
{
	mJumpSpeed = jumpSpeed;
}

// �U���͂��擾
int CCharaStatusBase::GetAttackPower() const
{
	return mAttackPower;
}

// �U���͂�ݒ�
void CCharaStatusBase::SetAttackPower(int attackPower)
{
	mAttackPower = attackPower;
}
