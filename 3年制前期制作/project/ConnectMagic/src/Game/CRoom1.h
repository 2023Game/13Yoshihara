#pragma once
#include "CRoomBase.h"

// ����1
class CRoom1 : public CRoomBase
{
public:
	// �R���X�g���N�^
	CRoom1(CVector pos);
	// �f�X�g���N�^
	~CRoom1();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
};