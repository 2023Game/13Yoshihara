#pragma once
#include "CFieldBase.h"

// �����̊��N���X
class CRoomBase : public CFieldBase
{
public:
	// �R���X�g���N�^
	CRoomBase(float roomLength);
	// �f�X�g���N�^
	~CRoomBase();

	// �����̒������擾
	float SetRoomLength() const;

private:
	// �����̒���
	float mRoomLength;
};