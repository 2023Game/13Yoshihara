#pragma once
#include "CFieldBase.h"

class CTrashField : public CFieldBase
{
public:
	// �R���X�g���N�^
	CTrashField();
	// �f�X�g���N�^
	~CTrashField();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	// �t�B�[���h�̃I�u�W�F�N�g����
	void CreateFieldObjects() override;
};