#include "CTable.h"

#define SCALE 1.5f

// �R���X�g���N�^
CTable::CTable()
{
	mpModel = CResourceManager::Get<CModel>("Table");

	Scale(Scale() * SCALE);

	// �R���C�_�[�𐶐�
	CreateCol("TableCol", "", "");
}

// �f�X�g���N�^
CTable::~CTable()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CTable::CreateFieldObjects()
{
}
