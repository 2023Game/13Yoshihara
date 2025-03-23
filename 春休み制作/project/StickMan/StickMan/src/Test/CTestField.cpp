#include "CTestField.h"

// �R���X�g���N�^
CTestField::CTestField()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TestField");

	// �I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CTestField::~CTestField()
{
}

// �t�B�[���h�̃I�u�W�F�N�g�𐶐�
void CTestField::CreateFieldObjects()
{
}

// �t�B�[���h�̃R���C�_�\�𐶐�
void CTestField::CreateCol()
{
	mpGroundColliderMesh = new CColliderMesh(this, ELayer::eGround, mpModel);
}
