#include "CDeliveryItemStatus.h"

#define SCORE 200	// �������̓��_
#define DAMAGE 2	// �^����_���[�W

// �R���X�g���N�^
CDeliveryItemStatus::CDeliveryItemStatus()
	: mScore(SCORE)
	, mDamage(DAMAGE)
{
}

// �f�X�g���N�^
CDeliveryItemStatus::~CDeliveryItemStatus()
{
}

// �������̓��_���擾
int CDeliveryItemStatus::GetScore() const
{
	return mScore;
}

// �^����_���[�W���擾
int CDeliveryItemStatus::GetDamage() const
{
	return mDamage;
}
