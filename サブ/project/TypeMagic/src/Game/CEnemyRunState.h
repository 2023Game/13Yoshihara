#pragma once
#include "CEnemyStateBase.h"

// �G�̓�����ԃN���X
class CEnemyRunState : public CEnemyStateBase
{
public:
	// �C���X�^���X
	static CEnemyRunState* Instance();

	// �R���X�g���N�^
	CEnemyRunState();
	// �f�X�g���N�^
	~CEnemyRunState();

	// �J�n���̏���
	void Start(CEnemy* enemy);
	// �X�V���̏���
	void Update(CEnemy* enemy);
	// �I�����̏���
	void End(CEnemy* enemy);
	// ���O���擾
	std::string GetName() const;
};