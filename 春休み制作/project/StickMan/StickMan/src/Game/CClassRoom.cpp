#include "CClassRoom.h"

#define SCALE 10.0f

// �R���X�g���N�^
CClassRoom::CClassRoom()
{
	mpModel = CResourceManager::Get<CModel>("ClassRoom");

	Scale(Scale() * SCALE);
}

// �f�X�g���N�^
CClassRoom::~CClassRoom()
{
}

// �t�B�[���h�I�u�W�F�N�g�̐���
void CClassRoom::CreateFieldObjects()
{
}
