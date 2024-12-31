#include "CTrashField.h"
 
// �R���X�g���N�^
CTrashField::CTrashField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TrashStage");

	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eField, CResourceManager::Get<CModel>("TrashStage_Ground_Collision"), true);
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, CResourceManager::Get<CModel>("TrashStage_Wall_Collision"), true);
	mpObjectColliderMesh = new CColliderMesh(this, ELayer::eObject, CResourceManager::Get<CModel>("TrashStage_Object_Collision"), true);

	CreateFieldObjects();
}

// �f�X�g���N�^
CTrashField::~CTrashField()
{
}

// �t�B�[���h�̃I�u�W�F�N�g����
void CTrashField::CreateFieldObjects()
{
}

// �X�V
void CTrashField::Update()
{
}

// �`��
void CTrashField::Render()
{
	mpModel->Render(Matrix());
}
