#include "CEnemyStateBase.h"
#include "CEnemy.h"

// 開始時の処理
void CEnemyStateBase::Start(CEnemy* enemy)
{
	enemy->SetStateStep(0);
	enemy->SetElapsedTime(0.0f);
}
