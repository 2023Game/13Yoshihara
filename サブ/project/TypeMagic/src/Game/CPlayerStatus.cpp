#include "CPlayerStatus.h"
#include "CharaStatusDefine.h"

// �R���X�g���N�^
CPlayerStatus::CPlayerStatus()
	: CCharaStatusBase(MAX_HP, MAX_MP, REGENE_MP, MOVE_SPEED, JUMP_SPEED)
{
}

// �f�X�g���N�^
CPlayerStatus::~CPlayerStatus()
{
}
