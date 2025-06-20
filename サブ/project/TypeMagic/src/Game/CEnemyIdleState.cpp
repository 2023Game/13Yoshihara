#include "CEnemyIdleState.h"
#include "CEnemy.h"

// �C���X�^���X
CEnemyIdleState* CEnemyIdleState::Instance()
{
	static CEnemyIdleState instance;
	return &instance;
}

// �R���X�g���N�^
CEnemyIdleState::CEnemyIdleState()
{
}

// �f�X�g���N�^
CEnemyIdleState::~CEnemyIdleState()
{
}

// �J�n���̏���
void CEnemyIdleState::Start(CEnemy* enemy)
{
	// ���N���X�̊J�n���̏���
	CEnemyStateBase::Start(enemy);
}

// �X�V���̏���
void CEnemyIdleState::Update(CEnemy* enemy)
{
}

// �I�����̏���
void CEnemyIdleState::End(CEnemy* enemy)
{
}

// ���O���擾
std::string CEnemyIdleState::GetName() const
{
	return "Idle";
}
