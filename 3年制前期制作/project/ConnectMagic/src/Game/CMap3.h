#pragma once
#include "CFieldBase.h"

class CMap3 : public CFieldBase
{
public:
	// �R���X�g���N�^
	CMap3();
	// �f�X�g���N�^
	~CMap3();

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �o�H�T���p�̃m�[�h�𐶐�
	void CreateNavNodes() override;
};