#pragma once
#include "CTrashStatusBase.h"

class CCarStatus : public CTrashStatusBase
{
public:
	CCarStatus();
	~CCarStatus();

	// ���ł܂ł̎��Ԃ��擾����
	float GetDeleteTime();
	// ���ł܂ł̎��Ԃ�ݒ肷��
	void SetDeleteTime(float deleteTime);
	// ���ł܂ł̎��Ԃ��o�߂������ǂ���
	bool IsElapsedDeleteTime();
	// ���ł܂ł̎��Ԃ��J�E���g
	void CountDeleteTime();
private:
	float mDeleteTime;	// ���ł܂ł̎���
};