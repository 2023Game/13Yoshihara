#include "CFieldManager.h"
#include "CMap_1.h"

// �}�b�v���m�̊Ԋu
#define MAP_DIST 200.0f

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

	// �}�b�v�𐶐�
	CreateMap();
}

// �f�X�g���N�^
CFieldManager::~CFieldManager()
{
	SAFE_DELETE(spInstance);
}

// �n�ʂ̃R���C�_�\�擾
CCollider* CFieldManager::GetGroundCol()
{
	return nullptr;
}

// �ǂ̃R���C�_�\�擾
CCollider* CFieldManager::GetWallCol()
{
	return nullptr;
}

// �I�u�W�F�N�g�̃R���C�_�\�擾
CCollider* CFieldManager::GetObjCol()
{
	return nullptr;
}

// �}�b�v�𐶐�
void CFieldManager::CreateMap()
{
	// �}�b�v���m���q����邩�ǂ���
	bool isUp = true;
	bool isDown = true;
	bool isLeft = true;
	bool isRight = true;
	for (int i = 0; i < 3; i++)
	{
		// ��ԏ�̍s�Ȃ��ւ͌q����Ȃ�
		if (i == 0)
		{
			isUp = false;
			isDown = true;
		}
		// ��ԉ��̍s�Ȃ牺�ւ͌q����Ȃ�
		else if (i == MAP_ROW - 1)
		{
			isUp = true;
			isDown = false;
		}
		// ����ȊO�͌q����
		else
		{
			isUp = true;
			isDown = true;
		}
		for (int j = 0; j < 3; j++)
		{
			// ��ԍ��̗�Ȃ獶�ւ͌q����Ȃ�
			if (j == 0)
			{
				isLeft = false;
				isRight = true;
			}
			// ��ԉE�̗�Ȃ�E�ւ͌q����Ȃ�
			else if (j == MAP_COL - 1)
			{
				isLeft = true;
				isRight = false;
			}
			// ����ȊO�͌q����
			else
			{
				isLeft = true;
				isRight = true;
			}
			// �}�b�v�𐶐�
			mpMapList[i][j] = new CMap_1(isUp, isDown, isLeft, isRight);
			// ���W��ݒ�
			mpMapList[i][j]->Position((j - 1) * MAP_DIST, 0.0f, (i - 1) * MAP_DIST);
			// �ǂ��ǂ𐶐�
			mpMapList[i][j]->CreateBlockWall();
		}
	}
}
