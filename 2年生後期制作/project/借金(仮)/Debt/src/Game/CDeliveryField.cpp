#include "CDeliveryField.h"

// �R���X�g���N�^
CDeliveryField::CDeliveryField()
	: CFieldBase()
{
	// �X�P�[����ݒ�
	Scale(Scale() * SCALE_RATIO);
	mpModel = CResourceManager::Get<CModel>("DeliveryStage");
	mpGroundColliderMesh = new CColliderMesh
	(
		this, ELayer::eGround,
		CResourceManager::Get<CModel>
		(
			"DeliveryStage_Ground_Collision"
		),
		true
	);
	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CDeliveryField::~CDeliveryField()
{
	SAFE_DELETE(mpGroundColliderMesh);
}

// �X�V
void CDeliveryField::Update()
{
	// �����ړ��̑��x���A��O�ֈړ�
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
}

// �`��
void CDeliveryField::Render()
{
	CFieldBase::Render();
}

// �t�B�[���h�̃I�u�W�F�N�g����
void CDeliveryField::CreateFieldObjects()
{

}