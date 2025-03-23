#include "CFieldManager.h"
#include "CMap_1.h"

// �t�B�[���h�Ǘ��N���X�̃C���X�^���X
CFieldManager* CFieldManager::spInstance = nullptr;

// �t�B�[���h�Ǘ��N���X�̃C���X�^���X���擾
CFieldManager* CFieldManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CFieldManager::CFieldManager()
{
	spInstance = this;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; i < 3; i++)
		{
			mMapList[i][j] = new CMap_1();
		}
	}
}

// �f�X�g���N�^
CFieldManager::~CFieldManager()
{
	SAFE_DELETE(spInstance);
}
