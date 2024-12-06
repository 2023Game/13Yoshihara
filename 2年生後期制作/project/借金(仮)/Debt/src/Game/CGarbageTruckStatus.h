#pragma once
#include "CCarStatus.h"

class CGarbageTruckStatus : public CCarStatus
{
public:
	CGarbageTruckStatus();
	~CGarbageTruckStatus();

	// �P�ނ܂ł̎��Ԃ��擾
	float GetBackTime();
	// �P�ނ܂ł̎��Ԃ�ݒ�
	void SetBackTime(float backTime);
	// �P�ނ܂ł̎��Ԃ��J�E���g
	void CountBackTime();

	// ����͈͂̔��a���擾����
	float GetCollectRadius();

	// ������̐l�����擾����
	int GetCollectors();
	// ������̐l����ݒ肷��
	void SetCollectors(int collectors);
private:
	float mBackTime;		// �P�ނ܂ł̎���
	float mCollectRadius;	// ����͈͂̔��a
	int mCollectors;		// ������̐l��
};