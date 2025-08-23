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
	float GetRoomLength() const;

	// �����̗L��������؂�ւ���
	virtual void SetEnableRoom(bool enable);

protected:
	// �����̒���
	float mRoomLength;
	// �I�u�W�F�N�g�̃��X�g
	std::vector<CObjectBase*> mObjs;
};