#include "CDeliveryField.h"

// �R���X�g���N�^
CDeliveryField::CDeliveryField()
	: CFieldBase()
{
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

}

// �f�X�g���N�^
CDeliveryField::~CDeliveryField()
{
}

// �X�V
void CDeliveryField::Update()
{
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
