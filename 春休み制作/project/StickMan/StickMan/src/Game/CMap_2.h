#pragma once
#include "CFieldBase.h"

class CMap_2 : public CFieldBase
{
public:
	// �R���X�g���N�^
	CMap_2();
	// �f�X�g���N�^
	~CMap_2();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �o�H�T���p�̃m�[�h�𐶐�
	void CreateNavNodes() override;
};