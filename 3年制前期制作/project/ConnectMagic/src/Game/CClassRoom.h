#pragma once
#include "CFieldBase.h"

// ����
class CClassRoom : public CFieldBase
{
public:
	// �R���X�g���N�^
	CClassRoom();
	// �f�X�g���N�^
	~CClassRoom();

private:
	// �t�B�[���h�I�u�W�F�N�g�̐���
	void CreateFieldObjects();
};