#include "CMap_3.h"

// �R���X�g���N�^
CMap_3::CMap_3()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("Map_3");

	// �R���C�_�[�𐶐�
	CreateCol("Map_3_GroundCol", "Map_3_WallCol", "");
}

// �f�X�g���N�^
CMap_3::~CMap_3()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CMap_3::CreateFieldObjects()
{
}

// �o�H�T���p�̃m�[�h�𐶐�
void CMap_3::CreateNavNodes()
{
}
