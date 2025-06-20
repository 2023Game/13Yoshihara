#pragma once
#include "CEnemyStateBase.h"

// �G�̉r������ԃN���X
class CEnemyCastState : public CEnemyStateBase
{
public:
	// �C���X�^���X
	static CEnemyCastState* Instance();

	// �R���X�g���N�^
	CEnemyCastState();
	// �f�X�g���N�^
	~CEnemyCastState();

	// �J�n���̏���
	void Start(CEnemy* enemy);
	// �X�V���̏���
	void Update(CEnemy* enemy);
	// �I�����̏���
	void End(CEnemy* enemy);
	// ���O���擾
	std::string GetName() const;
};