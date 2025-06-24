#include "CEnemyStatus.h"
#include "CharaStatusDefine.h"

// コンストラクタ
CEnemyStatus::CEnemyStatus()
	: CCharaStatusBase(MAX_HP, MAX_MP, REGENE_MP, MOVE_SPEED, JUMP_SPEED)
{
}

// デストラクタ
CEnemyStatus::~CEnemyStatus()
{
}
