#include "CEnemyChaseState.h"
#include "CEnemy.h"
#include "SpellDefine.h"

// �C���X�^���X
CEnemyChaseState* CEnemyChaseState::Instance()
{
	static CEnemyChaseState instance;
	return &instance;
}

// �R���X�g���N�^
CEnemyChaseState::CEnemyChaseState()
{
}

// �f�X�g���N�^
CEnemyChaseState::~CEnemyChaseState()
{
}

// �J�n���̏���
void CEnemyChaseState::Start(CEnemy* enemy)
{
	// ���N���X�̊J�n���̏���
	CEnemyStateBase::Start(enemy);
}

// �X�V���̏���
void CEnemyChaseState::Update(CEnemy* enemy)
{
	// �{�[�����@�̓͂�����
	float dist = BALL_SPEED * BALL_DELETE_TIME;
	// ����ւ̕���
	CVector moveDir = enemy->GetOpponent()->Position() - enemy->Position();
	moveDir.Normalize();
	// �ړ�������ݒ�
	enemy->SetMoveDir(moveDir);

	// �ړ������߂�
	CVector moveSpeed = enemy->GetMoveDir() * enemy->GetBaseMoveSpeed() * Times::DeltaTime();
	// �ړ���ݒ�
	enemy->SetMoveSpeed(moveSpeed);
}

// �I�����̏���
void CEnemyChaseState::End(CEnemy* enemy)
{
}

// ���O���擾
std::string CEnemyChaseState::GetName() const
{
	return "Chase";
}
