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
	// �o�߂�����
	enemy->AddElapsedTime(Times::DeltaTime());

	// �r�����Ԃ��o�߂�����
	if (enemy->GetElapsedTime() > CAST_SPEED)
	{
		// �r���\��̕�����
		std::string str;
		// �r���ς݂̕�������擾
		std::string castedStr = enemy->GetNewStr();

		// �r�����x������
		enemy->AddElapsedTime(-CAST_SPEED);

		// �X�e�b�v���擾
		int step = enemy->GetStateStep();
		switch (step)
		{
			// �����̉r��
		case 0:
			// �r�����镶������擾
			str = enemy->GetMainElementalStr();
			break;

			// �`�̉r��
		case 1:
			// �r�����镶������擾
			str = enemy->GetCastShapeStr();
			break;

			// �r��
		case 2:
			enemy->CastSpell();
			// ���������Ă���
			CEnemyStateBase::Start(enemy);
			// �œK�ȍs���ɕω�
			enemy->ChangeBestState();
			return;
			break;
		}

		// �r���\��̕����񂪎c���Ă�����
		if (str.size() > castedStr.size())
		{
			// �r���ς݂̎��̕���
			char c = str[castedStr.size()];
			// �r���ς݂̕�����ɒǉ�
			enemy->BasicCastSpell(std::string(1, c));
		}
		// �r���\��̕����񂪎c���Ă��Ȃ�
		else
		{
			// ���̃X�e�b�v��
			enemy->SetStateStep(step + 1);
			// �X�y�[�X��ǉ�����
			enemy->BasicCastSpell(" ");
			// �g���������Č��肷��
			enemy->SetCastShapeStr(CEnemyContext::Instance()->ScoreAtkSpell(enemy->GetContext()));
		}
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
