#include "CPlayerStatus.h"
#include "CharaStatusDefine.h"

// コンストラクタ
CPlayerStatus::CPlayerStatus()
	: CCharaStatusBase(MAX_HP, MAX_MP, REGENE_MP, MOVE_SPEED, JUMP_SPEED)
{
}

// デストラクタ
CPlayerStatus::~CPlayerStatus()
{
}
