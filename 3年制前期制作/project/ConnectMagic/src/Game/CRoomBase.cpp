#include "CRoomBase.h"

// �R���X�g���N�^
CRoomBase::CRoomBase(float roomLength)
	: CFieldBase()
{
}

// �f�X�g���N�^
CRoomBase::~CRoomBase()
{
}

// �����̒������擾
float CRoomBase::SetRoomLength() const
{
	return mRoomLength;
}
