#include "CTrashStatusBase.h"

CTrashStatusBase::CTrashStatusBase()
	: mTakeKnockback(0)
{
	mStatus =
	{
		0,
		0.0f,
		0.0f,
		0,
	};
}

CTrashStatusBase::~CTrashStatusBase()
{
}

// �_���[�W��1�󂯂�
void CTrashStatusBase::TakeDamage()
{
	mStatus.mHp--;
}

// �m�b�N�o�b�N�̒l���擾
float CTrashStatusBase::GetTakeKnockback()
{
	return mTakeKnockback;
}

// �󂯂�m�b�N�o�b�N���x��ݒ�
void CTrashStatusBase::SetTakeKnockback(float knockback)
{
	mTakeKnockback = knockback;
}

// �x�[�X�X�e�[�^�X���擾����
CTrashStatusBase::StatusBase CTrashStatusBase::GetStatusBase()
{
	return mStatus;
}

// �x�[�X�X�e�[�^�X��ݒ肷��
void CTrashStatusBase::SetStatusBase(int hp, float moveSpeed, float jumpSpeed, float knockback, int power)
{
	mStatus = 
	{
		hp,
		moveSpeed,
		jumpSpeed,
		knockback,
		power,
	};
}
