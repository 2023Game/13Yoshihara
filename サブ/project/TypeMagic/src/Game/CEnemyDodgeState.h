#pragma once
#include "CEnemyStateBase.h"

// �G�̉����ԃN���X
class CEnemyDodgeState : public CEnemyStateBase
{
public:
	// �C���X�^���X
	static CEnemyDodgeState* Instance();

	// �R���X�g���N�^
	CEnemyDodgeState();
	// �f�X�g���N�^
	~CEnemyDodgeState();

	// �J�n���̏���
	void Start(CEnemy* enemy);
	// �X�V���̏���
	void Update(CEnemy* enemy);
	// �I�����̏���
	void End(CEnemy* enemy);
	// ���O���擾
	std::string GetName() const;
};