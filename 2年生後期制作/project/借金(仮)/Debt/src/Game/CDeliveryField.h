#pragma once
#include "CFieldBase.h"

#define ROAD_X_AREA 87.5f	// �ԓ���X�͈̔�

class CDeliveryField : public CFieldBase
{
public:
	// �R���X�g���N�^
	CDeliveryField();
	// �f�X�g���N�^
	~CDeliveryField();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �t�B�[���h�̃I�u�W�F�N�g����
	void CreateFieldObjects() override;
};