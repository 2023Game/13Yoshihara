#include "CMap_1.h"

// �R���X�g���N�^
CMap_1::CMap_1()
{
	mpModel = CResourceManager::Get<CModel>("Map_1");
}

// �f�X�g���N�^
CMap_1::~CMap_1()
{
}

// �X�V
void CMap_1::Update()
{
}

// �`��
void CMap_1::Render()
{
	CFieldBase::Render();
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CMap_1::CreateFieldObjects()
{
}

// �o�H�T���p�̃m�[�h�𐶐�
void CMap_1::CreateNavNodes()
{
}
