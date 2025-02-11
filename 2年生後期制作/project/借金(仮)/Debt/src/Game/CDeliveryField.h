#pragma once
#include "CFieldBase.h"

// �X�P�[���̔{��
#define SCALE_RATIO 1.5f

// �ԓ���Z�͈̔�
#define ROAD_Z_AREA_P  170.0f	// �v���X����
#define ROAD_Z_AREA_M -300.0f	// �}�C�i�X����

// �ԓ���X���W
#define ROAD_LEFT1_POSX		-90.0f
#define ROAD_LEFT2_POSX		-30.0f
#define ROAD_RIGHT1_POSX	 90.0f
#define ROAD_RIGHT2_POSX	 30.0f

// �����ړ����x
#define FORCE_MOVE_SPEED 7.5f * 60.0f	

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