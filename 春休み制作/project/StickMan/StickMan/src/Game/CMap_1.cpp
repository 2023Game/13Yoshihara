#include "CMap_1.h"

// �R���X�g���N�^
CMap_1::CMap_1(bool isUp, bool isDown, bool isLeft, bool isRight)
	: CMapBase(isUp, isDown, isLeft, isRight)
{
	mpModel = CResourceManager::Get<CModel>("Book");

	// �R���C�_�[�𐶐�
	//CreateCol("Map_GroundCol", "Map_1_WallCol", "");
}

// �f�X�g���N�^
CMap_1::~CMap_1()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CMap_1::CreateFieldObjects()
{
}

// �o�H�T���p�̃m�[�h�𐶐�
void CMap_1::CreateNavNodes()
{
}