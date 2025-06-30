#include "CEnemyDodgeState.h"
#include "CEnemy.h"
#include "Maths.h"

// �������g��MP�̎c��
#define MP_THRESHOLD 0.7f

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
	CEnemyContext::EnemyContext context = enemy->GetContext();
	// MP�����򂩂�
	// ���������ł��Ă��Ȃ����߂��Ȃ�
	if (context.mpRatio > MP_THRESHOLD &&
		(!context.isSpellComing || !CEnemyContext::Instance()->IsNear(context.distanceToSpell)))
	{
		Cast(enemy, false);
	}
	else
	{
		// �����񂪎c���Ă�����r�����č폜
		enemy->CastSpell();
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

		// �œK�ȍs���ɕύX
		enemy->ChangeBestState();
	}
}

// �I�����̏���
void CEnemyDodgeState::End(CEnemy* enemy)
{
	// �����񂪎c���Ă�����r�����č폜
	enemy->CastSpell();
}

// ���O���擾
std::string CEnemyDodgeState::GetName() const
{
	return "Dodge";
}
