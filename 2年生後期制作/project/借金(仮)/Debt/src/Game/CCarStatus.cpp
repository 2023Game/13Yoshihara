#include "CCarStatus.h"

#define MAX_HP 0						// �ő�HP�i���g�p�j
#define BASE_MOVE_SPEED 0.375f * 5.0f	// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x�i���g�p�j
#define KNOCKBACK_SPEED 0.375f * 5.0f	// �m�b�N�o�b�N���x
#define POWER 1							// �S�~�𗎂Ƃ��p���[

#define DELETE_TIME 2.0f	// ���ł܂ł̎���

// �R���X�g���N�^
CCarStatus::CCarStatus()
	: CTrashStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, KNOCKBACK_SPEED, POWER)
	, mDeleteTime(DELETE_TIME)
{
}

/*
�R���X�g���N�^
���̃N���X���p�������L�����̃X�e�[�^�X�̐ݒ�p
*/
CCarStatus::CCarStatus(int maxHp, float baseMoveSpeed, float jumpSpeed, float knockback, int power)
	: CTrashStatusBase(maxHp, baseMoveSpeed,
		jumpSpeed, knockback, power)
	, mDeleteTime(DELETE_TIME)
{
}

CCarStatus::~CCarStatus()
{
}

// ���ł܂ł̎��Ԃ��擾����
float CCarStatus::GetDeleteTime() const
{
	return mDeleteTime;
}

// ���ł܂ł̎��Ԃ������l�ɐݒ肷��
void CCarStatus::SetDeleteTime()
{
	mDeleteTime = DELETE_TIME;
}

// ���ł܂ł̎��Ԃ��o�߂������ǂ���
bool CCarStatus::IsElapsedDeleteTime() const
{
	// ���ł܂ł̎��Ԃ�0���傫���Ȃ�o�߂��Ă��Ȃ�
	if (mDeleteTime > 0.0f)
	{
		return false;
	}
	// 0�ȉ��Ȃ�o�߂���
	else
	{
		return true;
	}
}

// ���ł܂ł̎��Ԃ��J�E���g�_�E��
void CCarStatus::CountDeleteTime()
{
	// 0���傫���Ȃ猸�Z
	if (mDeleteTime > 0.0f)
	{
		mDeleteTime -= Times::DeltaTime();
	}
}


