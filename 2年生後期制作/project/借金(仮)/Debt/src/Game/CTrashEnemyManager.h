#pragma once

class CTrashEnemy;
class CPunisherTrashEnemy;

// �S�~�E���̓G�Ǘ��N���X
class CTrashEnemyManager
{
public:
	// �G�Ǘ��N���X�̃C���X�^���X���擾
	static CTrashEnemyManager* Instance();
	// �R���X�g���N�^
	CTrashEnemyManager();
	// �f�X�g���N�^
	~CTrashEnemyManager();

	// �X�V
	void Update();

	// ���d�u���p�̓G���o�����Ă��邩
	bool GetPopPunisherEnemy();
	// ���d�u���p�̓G���o��������
	void PopPunisherEnemy();

private:
	// �S�~�E���̓G�Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CTrashEnemyManager* spInstance;

	// �ʏ�̓G
	CTrashEnemy* mpEnemy;
	// ���d�u���p�̓G
	CPunisherTrashEnemy* mpPunisherEnemy;

	// �o�ߎ���
	float mElapsedTime;
};