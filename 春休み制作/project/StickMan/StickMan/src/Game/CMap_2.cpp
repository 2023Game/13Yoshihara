#include "CMap_2.h"

// �R���X�g���N�^
CMap_2::CMap_2(bool isUp, bool isDown, bool isLeft, bool isRight)
	: CMapBase(isUp, isDown, isLeft, isRight)
{
	mpModel = CResourceManager::Get<CModel>("Map_2");

	// �R���C�_�[�𐶐�
	//CreateCol("Map_GroundCol", "Map_2_WallCol", "");
}

// �f�X�g���N�^
CMap_2::~CMap_2()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CMap_2::CreateFieldObjects()
{
}

// �o�H�T���p�̃m�[�h�𐶐�
void CMap_2::CreateNavNodes()
{
}
