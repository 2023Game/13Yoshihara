#include "CTrashBagStatus.h"

#define DELETE_TIME 5.0f	// ���ł܂ł̎���

#define NORMAL_POINT 100	// �ʏ�̃|�C���g
#define GOLD_POINT 1000		// �S�[���h�̃|�C���g

// �R���X�g���N�^
CTrashBagStatus::CTrashBagStatus(bool gold)
	: mIsGold(gold)
	, mDeleteTime(DELETE_TIME)
{
}

// �f�X�g���N�^
CTrashBagStatus::~CTrashBagStatus()
{
}

// �S�[���h���ǂ������擾����
bool CTrashBagStatus::GetIsGold() const
{
	return mIsGold;
}

// �S�[���h���ǂ�����ݒ肷��
void CTrashBagStatus::SetIsGold(bool isGold)
{
	mIsGold = isGold;
}

// ���ł܂ł̎��Ԃ��擾����
float CTrashBagStatus::GetDeleteTime() const
{
	return mDeleteTime;
}

// ���ł܂ł̎��Ԃ������l�ɐݒ肷��
void CTrashBagStatus::SetDeleteTime()
{
	mDeleteTime = DELETE_TIME;
}

// ���ł܂ł̎��Ԃ��o�߂������ǂ���
bool CTrashBagStatus::IsElapsedDeleteTime() const
{
	// ���ł܂ł̎��Ԃ�0�ȉ��Ȃ�o�߂���
	return mDeleteTime <= 0.0f;
}

// ���ł܂ł̎��Ԃ��J�E���g�_�E��
void CTrashBagStatus::CountDeleteTime()
{
	mDeleteTime -= Times::DeltaTime();
}

// ���_���擾����
int CTrashBagStatus::GetPoint(bool gold) const
{
	// �S�[���h�Ȃ�
	if (gold)
	{
		// �S�[���h�̃|�C���g��Ԃ�
		return GOLD_POINT;
	}
	// �ʏ�Ȃ�
	else
	{
		// �ʏ�̃|�C���g��Ԃ�
		return NORMAL_POINT;
	}
}
