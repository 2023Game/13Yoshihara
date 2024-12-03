#pragma once
#include "CTrashStatusBase.h"

class CGarbageTruckStatus : public CTrashStatusBase
{
public:
	CGarbageTruckStatus();
	~CGarbageTruckStatus();

	// �o���܂ł̎��Ԃ��擾����
	float GetPopTime();
	// �o���܂ł̎��Ԃ�ݒ肷��
	void SetPopTime(float popTime);

	// ���ł܂ł̎��Ԃ��擾����
	float GetDeleteTime();
	// ���ł܂ł̎��Ԃ�ݒ肷��
	void SetDeleteTime(float deleteTime);

	// �P�ނ܂ł̎��Ԃ��擾����
	float GetBackTime();
	// �P�ނ܂ł̎��Ԃ�ݒ肷��
	void SetBackTime(float backTime);

	// ����͈͂̔��a���擾����
	float GetCollectRadius();

	// ������̐l�����擾����
	int GetCollectors();
	// ������̐l����ݒ肷��
	void SetCollectors(int collectors);
protected:
	float mPopTime;			// �o���܂ł̎���
	float mDeleteTime;		// ���ł܂ł̎���
	float mBackTime;		// �P�ނ܂ł̎���
	float mCollectRadius;	// ����͈͂̔��a
	int mCollectors;		// ������̐l��
};