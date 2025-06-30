#include "CEnemyRunState.h"
#include "CEnemy.h"
#include "SpellDefine.h"

// �C���X�^���X
CEnemyRunState* CEnemyRunState::Instance()
{
	static CEnemyRunState instance;
	return &instance;
}

// �R���X�g���N�^
CEnemyRunState::CEnemyRunState()
{
}

// �f�X�g���N�^
CEnemyRunState::~CEnemyRunState()
{
}

// �J�n���̏���
void CEnemyRunState::Start(CEnemy* enemy)
{
	// ���N���X�̊J�n���̏���
	CEnemyStateBase::Start(enemy);
}

// �X�V���̏���
void CEnemyRunState::Update(CEnemy* enemy)
{
	// �����ւ̕���
	CVector moveDir = enemy->Position() - enemy->GetOpponent()->Position();
	moveDir.Normalize();
	enemy->SetMoveDir(moveDir);

	// �ړ���ݒ�
	CVector moveSpeed = enemy->GetMoveDir() * enemy->GetBaseMoveSpeed() * Times::DeltaTime();
	enemy->SetMoveSpeed(moveSpeed);

	// �œK�ȍs���ɕύX
	enemy->ChangeBestState();
}

// �I�����̏���
void CEnemyRunState::End(CEnemy* enemy)
{
}

// ���O���擾
std::string CEnemyRunState::GetName() const
{
	return "Run";
}
