#include "CResidentStatus.h"
#include "Maths.h"

#define MAX_HP 0						// �ő�HP
#define BASE_MOVE_SPEED 0.375f * 20.0f	// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x
#define ATTACK_POWER 0					// �U����
#define THROW_SPEED 2.2f				// �����鑬�x
#define THROW_TIME 1.0f					// �S�~�𓊂���܂ł̎���
#define MAX_THROW_NUM 3					// �������S�~�̍ő吔
#define GOLD_CHANCE 10					// �S�[���h�܂��o��m��

// �S�~�𓊂��鑬�x���v�Z����Ƃ������_���Ŋ|����ő�l
#define THROW_SPEED_RANDOM_MAX 1.5f
// �S�~�𓊂���܂ł̎��Ԃ��v�Z����Ƃ������_���Ŋ|����ő吔
#define THROW_TIME_RANDOM_MAX 5

// �R���X�g���N�^
CResidentStatus::CResidentStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED, JUMP_SPEED, ATTACK_POWER)
	, mThrowSpeed(THROW_SPEED)
	, mThrowTime(THROW_TIME)
	, mMaxThrowNum(MAX_THROW_NUM)
	, mGoldChance(GOLD_CHANCE)
{
	// �����_�����l���|�����l��ݒ�
	SetThrowTime();
}

// �f�X�g���N�^
CResidentStatus::~CResidentStatus()
{
}

// �S�~�܂𓊂��鑬�x��ݒ肷��
void CResidentStatus::SetThrowSpeed()
{
	// ���{����ő�l�܂ł̃����_���Ȑ��l���|����
	int random = Math::Rand(1.0f, THROW_SPEED_RANDOM_MAX);
	mThrowSpeed = THROW_SPEED * random;
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
	// ���{����ő吔�܂ł̃����_���Ȑ��l���|����
	int random = Math::Rand(1, THROW_TIME_RANDOM_MAX);
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

// �S�~�܂̍ő吔��ݒ�
void CResidentStatus::SetMaxThrowNum(int maxThrowNum)
{
	mMaxThrowNum = maxThrowNum;
}

// �S�~�܂̍ő吔���擾
int CResidentStatus::GetMaxThrowNum() const
{
	return mMaxThrowNum;
}

// �S�[���h�m����ݒ�
void CResidentStatus::SetGoldChance(int goldChance)
{
	mGoldChance = goldChance;
}

// �S�[���h�m�����擾
int CResidentStatus::GetGoldChance() const
{
	return mGoldChance;
}
