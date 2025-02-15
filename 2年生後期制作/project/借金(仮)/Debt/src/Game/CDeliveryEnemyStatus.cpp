#include "CDeliveryEnemyStatus.h"

#define MAX_HP 10						// �ő�HP
#define BASE_MOVE_SPEED 7.5f * 60.0f	// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x
#define ATTACK_POWER 1					// �U����
#define SCORE 5000						// ���_
#define THROW_SPEED	7.5f * 60.0f		// �����鑬�x

// Hp�������̈ړ����x�̌����l
#define SPEED_DOWN 7.5f

// �R���X�g���N�^
CDeliveryEnemyStatus::CDeliveryEnemyStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, ATTACK_POWER)
	, mScore(SCORE)
	, mThrowSpeed(THROW_SPEED)
{
}

// �f�X�g���N�^
CDeliveryEnemyStatus::~CDeliveryEnemyStatus()
{
}

// �󂵂���Ⴆ�链�_���擾
int CDeliveryEnemyStatus::GetScore() const
{
	return mScore;
}

// ��b�ړ����x���擾
float CDeliveryEnemyStatus::GetBaseMoveSpeed() const
{
	// ��b�ړ����x���擾
	float baseMoveSpeed = CCharaStatusBase::GetBaseMoveSpeed();
	// �������̌����l�~HP�A�������Ĉړ����x���v�Z
	float moveSpeed = baseMoveSpeed - SPEED_DOWN * GetHp();
	// 0�ȉ��Ȃ�0�ɂ���
	if (moveSpeed <= 0.0f) moveSpeed = 0.0f;
	return moveSpeed;
}

// �����鑬�x���擾
float CDeliveryEnemyStatus::GetThrowSpeed() const
{
	return mThrowSpeed;
}
