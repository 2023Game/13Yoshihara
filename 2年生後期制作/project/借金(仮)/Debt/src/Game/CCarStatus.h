#pragma once
#include "CTrashStatusBase.h"

class CCarStatus : public CTrashStatusBase
{
public:
	CCarStatus();
	~CCarStatus();

	// �o���܂ł̎��Ԃ��擾����
	float GetPopTime();
	// �o���܂ł̎��Ԃ�ݒ肷��
	void SetPopTime(float popTime);

	// ���ł܂ł̎��Ԃ��擾����
	float GetDeleteTime();
	// ���ł܂ł̎��Ԃ�ݒ肷��
	void SetDeleteTime(float deleteTime);
private:
	float mPopTime;		// �o���܂ł̎���
	float mDeleteTime;	// ���ł܂ł̎���
};