#include "CMap1.h"

// �����̉��̃I�t�Z�b�g���W
#define TORCH_FIRE_OFFSET_POS CVector(0.0f,9.0f,0.0f)

// �R���X�g���N�^
CMap1::CMap1()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("Map1");

	// �R���C�_�[�𐶐�
	CreateCol("Map1_GroundCol", "Map1_WallCol", "");
}

// �f�X�g���N�^
CMap1::~CMap1()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CMap1::CreateFieldObjects()
{
}

// �o�H�T���p�̃m�[�h�𐶐�
void CMap1::CreateNavNodes()
{
}
