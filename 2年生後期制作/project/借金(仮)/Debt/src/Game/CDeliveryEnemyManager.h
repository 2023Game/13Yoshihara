#pragma once

class CDeliveryEnemy;

// �z�B�̓G�Ǘ��N���X
class CDeliveryEnemyManager
{
public:
	// �C���X�^���X���擾
	static CDeliveryEnemyManager* Instance();
	// �R���X�g���N�^
	CDeliveryEnemyManager();
	// �f�X�g���N�^
	~CDeliveryEnemyManager();

	// �X�V
	void Update();

	// �G���o�����Ă��邩
	bool GetEnamyEnable();

private:
	// �C���X�^���X
	static CDeliveryEnemyManager* spInstance;
	// �����_���ȓ��ɓG�𐶐�
	void PopEnemyRandomRoad();
	// �G
	CDeliveryEnemy* mpEnemy;
	// �o�ߎ���
	float mElapsedTime;
};