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

// �����̗L��������؂�ւ���
void CRoomBase::SetEnableRoom(bool enable)
{
	SetEnable(enable);
	// �S�ẴI�u�W�F�̗L��������ݒ�
	for (CObjectBase* obj : mObjs)
	{
		obj->SetEnable(enable);
	}
}
