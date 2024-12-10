#include "CCarStatus.h"

#define MOVE_SPEED 0.375f * 5.0f		// �ړ����x
#define KNOCKBACK_SPEED 0.375f * 5.0f	// �m�b�N�o�b�N���x
#define POWER 1							// �S�~�𗎂Ƃ��p���[

#define DELETE_TIME 2.0f	// ���ł܂ł̎���

CCarStatus::CCarStatus()
	: CTrashStatusBase()
{
	// �x�[�X�X�e�[�^�X��ݒ�
	// �g��Ȃ��X�e�[�^�X�͏����l��0�̂܂�
	SetBaseMoveSpeed(MOVE_SPEED);
	SetKnockback(KNOCKBACK_SPEED);
	SetPower(POWER);

	// ���ł܂ł̎��Ԃ�ݒ�
	SetDeleteTime(DELETE_TIME);
}

CCarStatus::~CCarStatus()
{
}

// ���ł܂ł̎��Ԃ��擾����
float CCarStatus::GetDeleteTime() const
{
	return mDeleteTime;
}

// ���ł܂ł̎��Ԃ�ݒ肷��
void CCarStatus::SetDeleteTime(float deleteTime)
{
	mDeleteTime = deleteTime;
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

// ���ł܂ł̎��Ԃ��J�E���g
void CCarStatus::CountDeleteTime()
{
	// 0���傫���Ȃ猸�Z
	if (mDeleteTime > 0.0f)
	{
		mDeleteTime -= Times::DeltaTime();
	}
}


