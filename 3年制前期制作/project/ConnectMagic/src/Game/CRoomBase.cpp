#include "CRoomBase.h"

// �R���X�g���N�^
CRoomBase::CRoomBase(float roomLength)
	: CFieldBase()
	, mRoomLength(roomLength)
{
}

// �f�X�g���N�^
CRoomBase::~CRoomBase()
{
}

// �����̒������擾
float CRoomBase::GetRoomLength() const
{
	return mRoomLength;
}
