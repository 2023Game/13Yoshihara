#include "CEnemyCastState.h"
#include "CEnemy.h"

// �r���̑��x
#define CAST_SPEED 60.0f / 200.0f

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

	// �g�����������肷��
	enemy->SetCastShapeStr(CEnemyContext::Instance()->ScoreAtkSpell(enemy->GetContext()));
}

// �X�V���̏���
void CEnemyCastState::Update(CEnemy* enemy)
{
	// �����̉r��
	Cast(enemy, true);
}

// �I�����̏���
void CEnemyCastState::End(CEnemy* enemy)
{
	// �����񂪎c���Ă�����r�����č폜
	enemy->CastSpell();
	// ���N���X�̏I�����̏���
	CEnemyStateBase::End(enemy);
}

// ���O���擾
std::string CEnemyCastState::GetName() const
{
	return "Cast";
}
