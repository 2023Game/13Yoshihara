#pragma once
#include "CTrashStatusBase.h"

class CGarbageTruckStatus : public CTrashStatusBase
{
public:
	CGarbageTruckStatus();
	~CGarbageTruckStatus();
protected:
	float mPopTime;			// �o���܂ł̎���
	float mDeleteTime;		// ���ł܂ł̎���
	float mBackTime;		// �P�ނ܂ł̎���
	float mCollectRadius;	// ����͈͂̔��a
	int mCollectors;		// ������̐l��
};