#include "CTutorialMap.h"

// �R���X�g���N�^
CTutorialMap::CTutorialMap()
	: CFieldBase()
{
	mpModel = CResourceManager::Get<CModel>("TutorialMap");

	// �I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
	// �R���C�_�\�𐶐�
	CreateCol("TutorialMap_Ground_Col", "TutorialMap_Wall_Col", "");
}

// �f�X�g���N�^
CTutorialMap::~CTutorialMap()
{
}

// �t�B�[���h�̃I�u�W�F�N�g�𐶐�
void CTutorialMap::CreateFieldObjects()
{
}