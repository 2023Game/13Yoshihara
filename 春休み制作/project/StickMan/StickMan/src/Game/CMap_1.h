#pragma once
#include "CFieldBase.h"

class CMap_1 : public CFieldBase
{
public:
	// �R���X�g���N�^
	CMap_1();
	// �f�X�g���N�^
	~CMap_1();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	void CreateFieldObjects() override;
	// �o�H�T���p�̃m�[�h�𐶐�
	void CreateNavNodes() override;
};