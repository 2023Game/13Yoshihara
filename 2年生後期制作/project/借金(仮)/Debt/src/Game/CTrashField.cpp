#include "CTrashField.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "CVehicleManager.h"

// �R���X�g���N�^
CTrashField::CTrashField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TrashStage");

	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eField, CResourceManager::Get<CModel>("TrashStage_Ground_Collision"), true);
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, CResourceManager::Get<CModel>("TrashStage_Wall_Collision"), true);
	mpObjectColliderMesh = new CColliderMesh(this, ELayer::eObject, CResourceManager::Get<CModel>("TrashStage_Object_Collision"), true);

	// �t�B�[���h�̃I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
	// �o�H�T���p�̃m�[�h�𐶐�
	CreateNavNodes();
}

// �f�X�g���N�^
CTrashField::~CTrashField()
{
}

// �t�B�[���h�̃I�u�W�F�N�g����
void CTrashField::CreateFieldObjects()
{
}

// �o�H�T���p�̃m�[�h�𐶐�
void CTrashField::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		new CNavNode(CVector( 10.0f, 0.0f,  40.0f), this);
		new CNavNode(CVector(-10.0f, 0.0f,  40.0f), this);
		new CNavNode(CVector(-10.0f, 0.0f, -40.0f), this);
		new CNavNode(CVector( 10.0f, 0.0f, -40.0f), this);
	}
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
