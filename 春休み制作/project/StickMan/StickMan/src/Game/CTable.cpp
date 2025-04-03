#include "CTable.h"
#include "CMap_1.h"

#define SCALE 1.5f
#define ROT	CVector(0.0f,90.0f,0.0f)

// �R���X�g���N�^
CTable::CTable()
{
	mpModel = CResourceManager::Get<CModel>("Table");

	Scale(Scale() * SCALE);
	Rotation(ROT);

	// �R���C�_�[�𐶐�
	CreateCol("TableCol", "", "");

	// �t�B�[���h�I�u�W�F�N�g�𐶐�
	CreateFieldObjects();
}

// �f�X�g���N�^
CTable::~CTable()
{
}

// �t�B�[���h�I�u�W�F�N�g�𐶐�
void CTable::CreateFieldObjects()
{
	mpMap = new CMap_1(true, true, true, true);

	// �q���ǂ𐶐�
	mpMap->CreateConnectWall();
}
