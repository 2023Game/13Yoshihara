#pragma once

class CDeliveryEnemy;

// �z�B�̓G�Ǘ��N���X
class CDeliveryEnemyManager
{
public:
	// �R���X�g���N�^
	CDeliveryEnemyManager();
	// �f�X�g���N�^
	~CDeliveryEnemyManager();

	// �X�V
	void Update();

private:
	// �����_���ȓ��ɓG�𐶐�
	void PopEnemyRandomRoad();
	// �G
	CDeliveryEnemy* mpEnemy;
	// �o�ߎ���
	float mElapsedTime;
};