#pragma once
#include "CFieldBase.h"

class CTable : public CFieldBase
{
public:
	// �R���X�g���N�^
	CTable();
	// �f�X�g���N�^
	~CTable();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects();
};