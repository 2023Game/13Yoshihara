#include "CResidentManager.h"
#include "CResident.h"
#include "CNavNode.h"


// �Z�l�̐������̍��W
#define START_POS_LEFT  CVector(-100.0f,0.0f,-100.0f)
#define START_POS_RIGHT CVector( 100.0f,0.0f, 100.0f)

// ���̓��̏���|�C���g
#define PATROLPOINT_L_1 CVector(-100.0f, 4.0f, 100.0f)
#define PATROLPOINT_L_2 CVector(-100.0f, 4.0f,-100.0f)
// �E�̓��̏���|�C���g
#define PATROLPOINT_R_1 CVector(100.0f, 4.0f,-100.0f)
#define PATROLPOINT_R_2 CVector(100.0f, 4.0f, 100.0f)
 
// �Z�l�̍ő吔
#define RESIDENT_MAX_NUM 2

// �Z�l�Ǘ��N���X�̃C���X�^���X
CResidentManager* CResidentManager::spInstance = nullptr;

// �C���X�^���X�̃|�C���^�̎擾
CResidentManager* CResidentManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CResidentManager::CResidentManager()
{
	spInstance = this;

	// �Z�l�̃��f��
	mpResidentModel = CResourceManager::Get<CModelX>("Resident");

	// �o�H�T���p�̃m�[�h���쐬
	CreateNavNodes();

	// �Z�l�̍쐬
	CreateResident();
}

// �f�X�g���N�^
CResidentManager::~CResidentManager()
{
	// �C���X�^���X�ƍ폜���Ă���Z�l�Ǘ��N���X������Ȃ�폜
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

// �Z�l��S�Đ���
void CResidentManager::CreateResident()
{
	for (int i = 0; i < RESIDENT_MAX_NUM; i++)
	{
		// 2�Ŋ��������܂肪0�Ȃ獶�̓��ɐ���
		if (i % 2 == 0)
		{
			mpResidents.push_back(new CResident(mpResidentModel,START_POS_LEFT,mpPatrolPointsLeft));
		}
		// ���܂肪1�Ȃ�E�̓��ɐ���
		else
		{
			mpResidents.push_back(new CResident(mpResidentModel, START_POS_RIGHT, mpPatrolPointsRight));
		}
	}
}

// �o�H�T���p�̃m�[�h���쐬
void CResidentManager::CreateNavNodes()
{
	// ���̓��̏���|�C���g�𐶐�
	mpPatrolPointsLeft.push_back(new CNavNode(PATROLPOINT_L_1, true));
	mpPatrolPointsLeft.push_back(new CNavNode(PATROLPOINT_L_2, true));

	// �E�̓��̏���|�C���g�𐶐�
	mpPatrolPointsRight.push_back(new CNavNode(PATROLPOINT_R_1, true));
	mpPatrolPointsRight.push_back(new CNavNode(PATROLPOINT_R_2, true));
}
