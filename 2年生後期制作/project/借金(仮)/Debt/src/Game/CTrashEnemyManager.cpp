#include "CTrashEnemyManager.h"
#include "CTrashEnemy.h"
#include "CPunisherTrashEnemy.h"
#include "CTrashPlayer.h"
#include "CTrashField.h"

// �S�~�E���̓G�Ǘ��N���X�̃C���X�^���X
CTrashEnemyManager* CTrashEnemyManager::spInstance = nullptr;

// �ʏ�G�̏������W
#define ENEMY_POS CVector(0.0f,0.0f,-50.0f)
// �ʏ�G�̏�������
#define ENEMY_ROT CVector(0.0f,180.0f,0.0f)

// ���d�u���p�̓G�̐������̍��W
#define PUNISHER_POS_P CVector(0.0f,0.0f, 300.0f)	// Z���v���X�����̏ꍇ
#define PUNISHER_POS_M CVector(0.0f,0.0f,-300.0f)	// Z���}�C�i�X�����̏ꍇ
// ���d�u���p�̓G�̐������̕���
#define PUNISHER_ROT_P CVector(0.0f,180.0f,0.0f)	// Z���v���X�����̏ꍇ
#define PUNISHER_ROT_M CVector(0.0f,  0.0f,0.0f)	// Z���}�C�i�X�����̏ꍇ

// �o���܂ł̎���
#define PUNISHER_POP_TIME 3.0f

// �G�Ǘ��N���X�̃C���X�^���X���擾
CTrashEnemyManager* CTrashEnemyManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CTrashEnemyManager::CTrashEnemyManager()
	: mElapsedTime(0.0f)
{
	spInstance = this;
	// �ʏ�̓G�̐���
	mpEnemy = new CTrashEnemy();
	mpEnemy->Position(ENEMY_POS);
	mpEnemy->Rotation(ENEMY_ROT);

	// ���d�u���p�̓G�̐���
	mpPunisherEnemy = new CPunisherTrashEnemy();
	mpPunisherEnemy->Position(PUNISHER_POS_P);
	mpPunisherEnemy->Rotation(PUNISHER_ROT_P);
	// �ŏ��͖���
	mpPunisherEnemy->SetEnable(false);
	mpPunisherEnemy->SetShow(false);
}

// �f�X�g���N�^
CTrashEnemyManager::~CTrashEnemyManager()
{
	SAFE_DELETE(spInstance);
}

// �X�V
void CTrashEnemyManager::Update()
{
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
	// �G���A�O�̏ꍇ
	if (player->AreaOutZ())
	{
		mElapsedTime += Times::DeltaTime();
	}
	// �G���A���̏ꍇ
	else
	{
		mElapsedTime = 0.0f;
	}

	// �o���܂ł̎��Ԃ��o�߂��A
	// ���ɂ��d�u���p�̓G���o�����Ă��Ȃ��ꍇ
	if (mElapsedTime >= PUNISHER_POP_TIME &&
		!GetPopPunisherEnemy())
	{
		// ���d�u���p�̓G���o��
		PopPunisherEnemy();
		mElapsedTime = 0.0f;
	}
}

// ���d�u���p�̓G���o�����Ă��邩
bool CTrashEnemyManager::GetPopPunisherEnemy()
{
	return mpPunisherEnemy->IsEnable();
}

// ���d�u���p�̓G���o��������
void CTrashEnemyManager::PopPunisherEnemy()
{
	// �v���C���[�擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CTrashPlayer::Instance());
	CVector playerPos = player->Position();
	// �v���C���[���v���X�����ɂ���Ȃ�
	if (playerPos.Z() >= ROAD_Z_AREA)
	{
		mpPunisherEnemy->Position(PUNISHER_POS_P);
		mpPunisherEnemy->Rotation(PUNISHER_ROT_P);
	}
	// �}�C�i�X�����ɂ���Ȃ�
	else
	{
		mpPunisherEnemy->Position(PUNISHER_POS_M);
		mpPunisherEnemy->Rotation(PUNISHER_ROT_M);
	}
	// �L���ɂ���
	mpPunisherEnemy->SetEnable(true);
	mpPunisherEnemy->SetShow(true);
}
