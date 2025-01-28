#include "CTrashStatusBase.h"

// �R���X�g���N�^
CTrashStatusBase::CTrashStatusBase(int maxHp, float baseMoveSpeed, float jumpSpeed,
	float knockbackDealt, int attackPower, int power)
	: CCharaStatusBase(maxHp, baseMoveSpeed, jumpSpeed, attackPower)
	, mKnockbackDealt(knockbackDealt)
	, mPower(power)
	, mKnockbackReceived(CVector::zero)
	, mTrashBagNum(0)
	, mGoldTrashBagNum(0)
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

// �S�~�܂̐������Z����
void CTrashStatusBase::SetTrashBag(int num)
{
	mTrashBagNum += num;
	// 0��菬�����Ȃ�����0�ɂ���
	if (mTrashBagNum < 0) mTrashBagNum = 0;
}

// �S�~�܂̐����擾����
int CTrashStatusBase::GetTrashBag() const
{
	return mTrashBagNum;
}

// �S�[���h�S�~�܂̐������Z����
void CTrashStatusBase::SetGoldTrashBag(int num)
{
	mGoldTrashBagNum += num;
	// 0��菬�����Ȃ�����0�ɂ���
	if (mGoldTrashBagNum < 0) mGoldTrashBagNum = 0;
}

// �S�[���h�S�~�܂̐����擾����
int CTrashStatusBase::GetGoldTrashBag() const
{
	return mGoldTrashBagNum;
}
