#pragma once
#include "CFieldBase.h"

class CMap1 : public CFieldBase
{
public:
	// �R���X�g���N�^
	CMap1();
	// �f�X�g���N�^
	~CMap1();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �o�H�T���p�̃m�[�h�𐶐�
	void CreateNavNodes() override;
};