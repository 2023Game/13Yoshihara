#include "CEnemyManager.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "Maths.h"

#define DEFAULT_ENEMY_MAX_NUM 1		// �ő�ŏo������G�̐��̏����l
#define DEFAULT_ENEMY_ONCE_NUM 1	// ���ŏo������G�̐��̏����l	

// �o������Ԋu
#define POP_RATE 5.0f

// ��������鋗��
#define ENEMY_DIST 750.0f

// ���C���̏o������
#define MAIN_POP_DIR 8
// �T�u�̏o������
#define SUB_POP_DIR 3

// �G�̊Ǘ��N���X�̃C���X�^���X
CEnemyManager* CEnemyManager::spInstance = nullptr;

// �C���X�^���X�̃|�C���^�̎擾
CEnemyManager* CEnemyManager::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CEnemyManager::CEnemyManager()
	: mEnemyMaxNum(DEFAULT_ENEMY_MAX_NUM)
	, mEnemyOnceNum(DEFAULT_ENEMY_ONCE_NUM)
	, mWaveNum(1)
	, mElapsedTime(POP_RATE)
{
	spInstance = this;
	CVector playerPos = CPlayer::Instance()->Position();
	// �G���o�����������ۑ�
	for (int i = 0; i < MAIN_POP_DIR; i++)
	{
		// ���C���̏o�������̊p�x
		float baseAngle = i * (360 / MAIN_POP_DIR);
		for (int j = -1; j <= 1; j++)
		{
			// ���C���ƃT�u�𑫂����o������
			float angle = baseAngle + j * (360 / (MAIN_POP_DIR * SUB_POP_DIR));
			// ���W�A���p�ɕϊ�
			angle = Math::DegreeToRadian(angle);
			// ������������
			CVector dir = CVector(cosf(angle), 0.0f, sinf(angle));
			// �������W
			CVector pos = playerPos + dir * ENEMY_DIST;
			// �ۑ�
			mPopPos.push_back(pos);
		}
	}
}

// �f�X�g���N�^
CEnemyManager::~CEnemyManager()
{
	if (spInstance == this)
	{
		SAFE_DELETE(spInstance);
	}
}

// �X�V
void CEnemyManager::Update()
{
	mElapsedTime += Times::DeltaTime();

	// �����Ԋu���o�߂��Ă��邩��
	// �G�̐����ő�l��菭�Ȃ��ꍇ
	if (mElapsedTime >= POP_RATE &&
		mEnemys.size() < mEnemyMaxNum)
	{
		// �G��ǉ�
		AddEnemy();
		mElapsedTime -= POP_RATE;
	}
}

// �G�̐��𑝉�
void CEnemyManager::AddEnemy()
{
	// �v���C���[���擾
	CPlayer* player = dynamic_cast<CPlayer*>(CPlayer::Instance());
	// �G�𐶐�
	CEnemy* enemy = new CEnemy();
	enemy->SetOpponent(player);
	// �o���ꏊ���烉���_��
	int rand = Math::Rand(0, MAIN_POP_DIR + SUB_POP_DIR);
	CVector pos = mPopPos[rand];
	// ���W��ݒ�
	enemy->Position(pos);
	// �G�̃��X�g�ɒǉ�
	mEnemys.push_back(enemy);
	// �G�̐���1�ɂȂ������̓v���C���[�̃^�[�Q�b�g�ɐݒ肷��
	if (mEnemys.size() == 1)
	{
		player->SetOpponent(enemy);
	}
}

// �G���폜
void CEnemyManager::DeleteEnemy(CEnemy* enemy)
{
	auto it = mEnemys.begin();

	while (it != mEnemys.end())
	{
		// ��v����Ȃ�
		if (*it == enemy)
		{
			// �^�X�N����폜
			(*it)->Kill();
			// ���X�g����폜
			it = mEnemys.erase(it);
		}
		// ��v���Ȃ��Ȃ�
		else
		{
			// ����
			it++;
		}
	}
}
