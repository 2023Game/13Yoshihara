#include "CEnemyStateBase.h"
#include "CEnemy.h"

// ŠJŽnŽž‚Ìˆ—
void CEnemyStateBase::Start(CEnemy* enemy)
{
	enemy->SetStateStep(0);
	enemy->SetElapsedTime(0.0f);
}
