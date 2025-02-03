#pragma once
#include "CFieldBase.h"

class CVehicleBase;

#define ROAD_X_AREA 90.0f	// �ԓ���X�͈̔�
#define ROAD_Z_AREA 200.0f	// �ԓ���Z�͈̔�

class CTrashField : public CFieldBase
{
public:
	// �R���X�g���N�^
	CTrashField();
	// �f�X�g���N�^
	~CTrashField();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �t�B�[���h�̃I�u�W�F�N�g����
	void CreateFieldObjects() override;
	// �o�H�T���p�̃m�[�h�𐶐�
	void CreateNavNodes() override;
};