#include "CResidentStatus.h"
#include "Maths.h"

#define MAX_HP 0						// �ő�HP
#define BASE_MOVE_SPEED 0.375f * 20.0f	// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x
#define ATTACK_POWER 0					// �U����
#define THROW_SPEED 1.0f				// �����鑬�x
#define THROW_TIME 5.0f					// �S�~�𓊂���܂ł̎���

// �R���X�g���N�^
CResidentStatus::CResidentStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED, ATTACK_POWER)
	, mThrowSpeed(THROW_SPEED)
	, mThrowTime(THROW_TIME)
{
	// �����_�����l���|�����l��ݒ�
	SetThrowTime();
}

// �f�X�g���N�^
CResidentStatus::~CResidentStatus()
{
}

// �S�~�܂𓊂��鑬�x��ݒ肷��
void CResidentStatus::SetThrowSpeed(float throwSpeed)
{
	mThrowSpeed = throwSpeed;
}

// �S�~�܂𓊂��鑬�x���擾����
float CResidentStatus::GetThrowSpeed() const
{
	return mThrowSpeed;
}

// �S�~�𓊂���܂ł̎��Ԃ��擾
float CResidentStatus::GetThrowTime() const
{
	return mThrowTime;
}

// �S�~�𓊂���܂ł̎��Ԃ������l�Ƀ����_�����l���|�������ɐݒ�
void CResidentStatus::SetThrowTime()
{
	// ���{����1.5�{�܂ł̃����_���Ȑ��l���|����
	float random = Math::Rand(1.0f, 2.0f);
	mThrowTime = THROW_TIME * random;
}

// �S�~�𓊂���܂ł̎��Ԃ��o�߂������ǂ���
bool CResidentStatus::IsElapsedThrowTime() const
{
	// 0�ȉ��Ȃ�o�߂��Ă���
	return mThrowTime <= 0.0f;
}

// �S�~�𓊂���܂ł̎��Ԃ��J�E���g�_�E��
void CResidentStatus::CountThrowTime()
{
	mThrowTime -= Times::DeltaTime();
}
