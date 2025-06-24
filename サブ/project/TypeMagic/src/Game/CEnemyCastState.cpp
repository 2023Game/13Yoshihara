#include "CEnemyCastState.h"
#include "CEnemy.h"

// �r���̑��x
#define CAST_SPEED 1 / 5.6f

// �C���X�^���X
CEnemyCastState* CEnemyCastState::Instance()
{
	static CEnemyCastState instance;
	return &instance;
}

// �R���X�g���N�^
CEnemyCastState::CEnemyCastState()
{
}

// �f�X�g���N�^
CEnemyCastState::~CEnemyCastState()
{
}

// �J�n���̏���
void CEnemyCastState::Start(CEnemy* enemy)
{
	// ���N���X�̊J�n���̏���
	CEnemyStateBase::Start(enemy);
}

// �X�V���̏���
void CEnemyCastState::Update(CEnemy* enemy)
{
	// �������擾
	ESpellElementalType elemental = enemy->GetMainElemental();
	// �`���擾
	ESpellShapeType shape = enemy->GetCastShape();
	// �o�߂�����
	enemy->AddElapsedTime(Times::DeltaTime());

	// �r�����Ԃ��o�߂�����
	if (enemy->GetElapsedTime() > CAST_SPEED)
	{
		// �r�����x������
		enemy->AddElapsedTime(-CAST_SPEED);
	}
}

// �I�����̏���
void CEnemyCastState::End(CEnemy* enemy)
{
}

// ���O���擾
std::string CEnemyCastState::GetName() const
{
	return "Cast";
}
