#pragma once
#include "CTrashStatusBase.h"

class CCarStatus : public CTrashStatusBase
{
public:
	CCarStatus();
	~CCarStatus();
protected:
	float mPopTime;		// �o���܂ł̎���
	float mDeleteTime;	// ���ł܂ł̎���
};