#include "CDeliveryEnemyManager.h"
#include "CDeliveryEnemy.h"
#include "CDeliveryField.h"
#include "Maths.h"

// �G�̐����ʒu
#define ENEMY_POP_POSZ 300.0f

// �G�̐����Ԋu
#define ENEMY_POP_TIME 20.0f

CDeliveryEnemyManager* CDeliveryEnemyManager::spInstance = nullptr;

// �C���X�^���X���擾
CDeliveryEnemyManager* CDeliveryEnemyManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CDeliveryEnemyManager::CDeliveryEnemyManager()
	: mElapsedTime(0.0f)
{
	spInstance = this;
	// �G�̐���
	mpEnemy = new CDeliveryEnemy();
	// �ŏ��͖���
	mpEnemy->SetOnOff(false);
}

// �f�X�g���N�^
CDeliveryEnemyManager::~CDeliveryEnemyManager()
{
	SAFE_DELETE(spInstance);
}

// �X�V
void CDeliveryEnemyManager::Update()
{
	// �G�������Ȃ�
	if (!mpEnemy->IsEnable())
	{
		// ���Ԃ̌o��
		mElapsedTime += Times::DeltaTime();
		// �G�̐����܂ł̎��Ԃ��o�߂�����
		if (mElapsedTime >= ENEMY_POP_TIME)
		{
			mElapsedTime = 0.0f;
			// �����_���ȓ��ɓG�𐶐�
			PopEnemyRandomRoad();
		}
	}
}

// �G���o�����Ă��邩
bool CDeliveryEnemyManager::GetEnamyEnable()
{
	return mpEnemy->IsEnable();
}

// �����_���ȓ��ɓG�𐶐�
void CDeliveryEnemyManager::PopEnemyRandomRoad()
{
	// 1����4�̗���
	int random = Math::Rand(1, 4);
	switch (random)
	{
		// ��1�̓�
	case 1:
		mpEnemy->SetRoadType(ERoadType::eLeft1);
		mpEnemy->Position(ROAD_LEFT1_POSX, 0.0f, ENEMY_POP_POSZ);
		break;
		// ��2�̓�
	case 2:
		mpEnemy->SetRoadType(ERoadType::eLeft2);
		mpEnemy->Position(ROAD_LEFT2_POSX, 0.0f, ENEMY_POP_POSZ);
		break;
		// �E1�̓�
	case 3:
		mpEnemy->SetRoadType(ERoadType::eRight1);
		mpEnemy->Position(ROAD_RIGHT1_POSX, 0.0f, ENEMY_POP_POSZ);
		break;
		// �E2�̓�
	case 4:
		mpEnemy->SetRoadType(ERoadType::eRight2);
		mpEnemy->Position(ROAD_RIGHT2_POSX, 0.0f, ENEMY_POP_POSZ);
		break;
	}
	// �L��
	mpEnemy->SetOnOff(true);
}
