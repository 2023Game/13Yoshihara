#include "CEnemyDodgeState.h"
#include "CEnemy.h"
#include "Maths.h"

// �C���X�^���X
CEnemyDodgeState* CEnemyDodgeState::Instance()
{
	static CEnemyDodgeState instance;
	return &instance;
}

// �R���X�g���N�^
CEnemyDodgeState::CEnemyDodgeState()
{
}

// �f�X�g���N�^
CEnemyDodgeState::~CEnemyDodgeState()
{
}

// �J�n���̏���
void CEnemyDodgeState::Start(CEnemy* enemy)
{
	// ���N���X�̊J�n���̏���
	CEnemyStateBase::Start(enemy);
}

// �X�V���̏���
void CEnemyDodgeState::Update(CEnemy* enemy)
{
	// ����ւ̕���
	CVector dir = enemy->GetOpponent()->Position() - enemy->Position();
	dir.Normalize();
	// �ړ��̕���
	CVector moveDir;
	// �����̈ړ�����
	CVector spellMoveDir = enemy->GetComingSpellMoveDir();
	// �ړ�������ݒ�
	moveDir = CVector(-spellMoveDir.Z(), 0.0f, spellMoveDir.X());

	moveDir.Normalize();
	// �ړ�������ݒ�
	enemy->SetMoveDir(moveDir);
	// �ړ���ݒ�
	CVector moveSpeed = enemy->GetMoveDir() * enemy->GetBaseMoveSpeed() * Times::DeltaTime();
	enemy->SetMoveSpeed(moveSpeed);
}

// �I�����̏���
void CEnemyDodgeState::End(CEnemy* enemy)
{
}

// ���O���擾
std::string CEnemyDodgeState::GetName() const
{
	return "Dodge";
}
