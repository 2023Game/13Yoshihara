#include "CDeliveryHome.h"
#include "CDeliveryField.h"
#include "CColliderMesh.h"

// �R���X�g���N�^
CDeliveryHome::CDeliveryHome()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
{
	// ���f���擾
	mpModel = CResourceManager::Get<CModel>("DeliveryHome");
	// �ǂ̃R���C�_�\��ݒ�
	mpWallCol = new CColliderMesh
	(
		this, ELayer::eWall,
		CResourceManager::Get<CModel>("DeliveryHome_Wall_Collision"),
		true
	);
	// �S�[������̃R���C�_�\�ݒ�
	mpGoalCol = new CColliderMesh
	(
		this, ELayer::eGoal,
		CResourceManager::Get<CModel>("DeliveryHome_Goal_Collision"),
		true
	);
}

// �f�X�g���N�^
CDeliveryHome::~CDeliveryHome()
{
	SAFE_DELETE(mpWallCol);
	SAFE_DELETE(mpGoalCol);
}

// �X�V
void CDeliveryHome::Update()
{
	// �����ړ��̑��x���A��O�ֈړ�
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
}

// �`��
void CDeliveryHome::Render()
{
	mpModel->Render(Matrix());
}
