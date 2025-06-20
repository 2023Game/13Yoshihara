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

	switch (enemy->GetStateStep())
	{
	case 0:
	{
		// �����_�����l
		int rand = Math::Rand(1, 2);

		// �����_���ō��E�̉�����������
		switch (rand)
		{
		case 1:
			moveDir = CVector::up.Cross(dir);
			break;
		case 2:
			moveDir = dir.Cross(CVector::up);
			break;
		}

		// �����_���̐��l�̃X�e�b�v�֐i��
		enemy->SetStateStep(rand);
		break;
	}
	// �������̍X�V
	case 1:
		moveDir = CVector::up.Cross(dir);
		break;

		// �������̍X�V
	case 2:
		moveDir = dir.Cross(CVector::up);
		break;
	}

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
