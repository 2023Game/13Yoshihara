#include "CDeliveryEnemyStatus.h"

#define MAX_HP 1						// �ő�HP
#define BASE_MOVE_SPEED 7.5f * 60.0f	// �ړ����x
#define JUMP_SPEED 0.0f					// �W�����v���x
#define ATTACK_POWER 0					// �U����
#define SCORE 1000	// ���_

// �R���X�g���N�^
CDeliveryEnemyStatus::CDeliveryEnemyStatus()
	: CCharaStatusBase(MAX_HP, BASE_MOVE_SPEED,
		JUMP_SPEED, ATTACK_POWER)
	, mScore(SCORE)
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
