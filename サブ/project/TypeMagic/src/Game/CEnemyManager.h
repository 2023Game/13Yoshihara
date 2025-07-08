#pragma once
#include "CEnemy.h"

// �G�̊Ǘ��N���X
class CEnemyManager
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CEnemyManager* Instance();

	// �R���X�g���N�^
	CEnemyManager();
	// �f�X�g���N�^
	~CEnemyManager();

	// �X�V
	void Update();

	// �G�̐��𑝉�
	void AddEnemy();
	// �G���폜
	void DeleteEnemy(CEnemy* enemy);

private:
	// �G�̊Ǘ��N���X�̃C���X�^���X
	static CEnemyManager* spInstance;

	// �G�̍ő吔
	int mEnemyMaxNum;
	// ���ŏo������G�̐�
	int mEnemyOnceNum;
	// �E�F�[�u��
	int mWaveNum;
	// ���Ԍo��
	float mElapsedTime;

	// �G�̃��X�g
	std::vector<CEnemy*> mEnemys;
	// �G�̏o���ʒu
	std::vector<CVector> mPopPos;
};