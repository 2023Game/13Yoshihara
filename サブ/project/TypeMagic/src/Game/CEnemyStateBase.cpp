#include "CEnemyStateBase.h"
#include "CEnemy.h"

// �J�n���̏���
void CEnemyStateBase::Start(CEnemy* enemy)
{
	enemy->SetStateStep(0);
	enemy->SetElapsedTime(0.0f);
}
