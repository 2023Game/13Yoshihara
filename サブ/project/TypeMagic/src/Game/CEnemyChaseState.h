#pragma once
#include "CEnemyStateBase.h"

// �G�̒ǐՏ�ԃN���X
class CEnemyChaseState : public CEnemyStateBase
{
public:
	// �C���X�^���X
	static CEnemyChaseState* Instance();

	// �R���X�g���N�^
	CEnemyChaseState();
	// �f�X�g���N�^
	~CEnemyChaseState();

	// �J�n���̏���
	void Start(CEnemy* enemy);
	// �X�V���̏���
	void Update(CEnemy* enemy);
	// �I�����̏���
	void End(CEnemy* enemy);
	// ���O���擾
	std::string GetName() const;
};