#pragma once
#include "CEnemyStateBase.h"

// �G�̑ҋ@��ԃN���X
class CEnemyIdleState : public CEnemyStateBase
{
public:
	// �C���X�^���X
	static CEnemyIdleState* Instance();

	// �R���X�g���N�^
	CEnemyIdleState();
	// �f�X�g���N�^
	~CEnemyIdleState();

	// �J�n���̏���
	void Start(CEnemy* enemy);
	// �X�V���̏���
	void Update(CEnemy* enemy);
	// �I�����̏���
	void End(CEnemy* enemy);
	// ���O���擾
	std::string GetName() const;
};