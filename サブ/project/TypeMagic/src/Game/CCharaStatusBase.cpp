#include "CCharaStatusBase.h"

// �R���X�g���N�^
CCharaStatusBase::CCharaStatusBase(int maxHp, int maxMp, int regeneMp,
	float baseMoveSpeed, float jumpSpeed)
	: mMaxHp(maxHp)
	, mHp(mMaxHp)
	, mMaxMp(maxMp)
	, mMp(mMaxMp)
	, mRegeneMp(regeneMp)
	, mBaseMoveSpeed(baseMoveSpeed)
	, mJumpSpeed(jumpSpeed)
{
}

// �f�X�g���N�^
CCharaStatusBase::~CCharaStatusBase()
{
}

// �_���[�W���󂯂�
void CCharaStatusBase::TakeDamage(int damage, CObjectBase* attacker)
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

// MP���g�p
bool CCharaStatusBase::UseMp(int useMp)
{
	// MP������Ȃ��Ȃ玸�s
	if (mMp < useMp) return false;

	// ��������炷
	mMp -= useMp;

	// ����
	return true;
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

// ����HP�������Z����
void CCharaStatusBase::SetHp(int num)
{
	mHp += num;
	// Hp��0�ȉ��Ȃ�0�ɂ���
	if (mHp <= 0)
	{
		mHp = 0;
	}
}

// ����HP�����Z�b�g
void CCharaStatusBase::ResetHp()
{
	mHp = mMaxHp;
}

// �ő�MP���擾
float CCharaStatusBase::GetMaxMp() const
{
	return mMaxMp;
}

// �ő�MP��ݒ�
void CCharaStatusBase::SetMaxMp(float maxMp)
{
	mMaxMp = maxMp;
}

// ����MP���擾
float CCharaStatusBase::GetMp() const
{
	return mMp;
}

// ����MP�������Z����
void CCharaStatusBase::SetMp(float num)
{
	mMp += num;
	// Mp��0�ȉ��Ȃ�0
	if (mMp <= 0)
	{
		mMp = 0;
	}
	// Mp���ő�l���傫���Ȃ�ő�l
	else if (mMp > mMaxMp)
	{
		mMp = mMaxMp;
	}
}

// ����MP�����Z�b�g
void CCharaStatusBase::ResetMp()
{
	mMp = mMaxMp;
}

// MP�̍Đ�
void CCharaStatusBase::RegeneMp()
{
	SetMp(mRegeneMp * Times::DeltaTime());
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