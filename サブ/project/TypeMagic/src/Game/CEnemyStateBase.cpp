#include "CEnemyStateBase.h"
#include "CEnemy.h"

// �r���̑��x
#define CAST_SPEED 60.0f / 200.0f

// �J�n���̏���
void CEnemyStateBase::Start(CEnemy* enemy)
{
	enemy->SetStateStep(0);
	enemy->SetElapsedTime(0.0f);
}

// �����r���̏���
void CEnemyStateBase::Cast(CEnemy* enemy, bool atkSpell)
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
			// �������Ȃ玟��
			if (str == "neutral")
			{
				// ���̃X�e�b�v��
				enemy->SetStateStep(step + 1);
				return;
			}
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
			// �U�������Ȃ�
			if (atkSpell)
			{
				// �g���������Č��肷��
				enemy->SetCastShapeStr(CEnemyContext::Instance()->ScoreAtkSpell(enemy->GetContext()));
			}
			// �U�������łȂ��Ȃ�
			else
			{
				// �g���������Č��肷��
				enemy->SetCastShapeStr(CEnemyContext::Instance()->ScoreSpSpell(enemy->GetContext()));
			}
		}
	}
}
