#pragma once
#include "CFieldBase.h"

// �e�X�g�p�̃t�B�[���h
class CTestField : public CFieldBase
{
public:
	// �R���X�g���N�^
	CTestField();
	// �f�X�g���N�^
	~CTestField();
private:
	// �t�B�[���h�̃I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
};