//�v���C���[�N���X�̃C���N���[�h
#include "CTrashPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"

// �v���C���[�̃C���X�^���X
CTrashPlayer* CTrashPlayer::spInstance = nullptr;

/*
 �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
 (��)���W�����Ă����Ԃ���̃A�j���[�V����
 (�J)���W���J���Ă����Ԃ���̃A�j���[�V����
 (�J��)�������̏�Ԍ��p�̃A�j���[�V����
*/
 const CTrashPlayer::AnimData CTrashPlayer::ANIM_DATA[] =
{
	{ "",												true,	0.0f	},	// T�|�[�Y
	{ "Character\\TrashBox\\anim\\IdleClose.x",			true,	10.0f	},	// �ҋ@					�i�Ɓj
	{ "Character\\TrashBox\\anim\\IdleOpen.x",			true,	10.0f	},	// �ҋ@					�i�J�j
	{ "Character\\TrashBox\\anim\\MoveClose.x",			true,	32.0f	},	// �ړ�					�i�Ɓj
	{ "Character\\TrashBox\\anim\\MoveOpen.x",			true,	32.0f	},	// �ړ�					�i�J�j
	{ "Character\\TrashBox\\anim\\DamageStartClose.x",	false,	4.0f	},	// ��e�J�n				�i�Ɓj
	{ "Character\\TrashBox\\anim\\DamageStartOpen.x",	false,	4.0f	},	// ��e�J�n				�i�J�j
	{ "Character\\TrashBox\\anim\\DamageClose.x",		false,	4.0f	},	// ��e�m�b�N�o�b�N		�i�Ɓj
	{ "Character\\TrashBox\\anim\\DamageOpen.x",		false,	4.0f	},	// ��e�m�b�N�o�b�N		�i�J�j
	{ "Character\\TrashBox\\anim\\DamageEndClose.x",	false,	28.0f	},	// ��e�I��				�i�Ɓj
	{ "Character\\TrashBox\\anim\\DamageEndOpen.x",		false,	28.0f	},	// ��e�I��				�i�J�j
	{ "Character\\TrashBox\\anim\\JumpStartClose.x",	false,	12.0f	},	// �W�����v�J�n			�i�Ɓj
	{ "Character\\TrashBox\\anim\\JumpStartOpen.x",		false,	12.0f	},	// �W�����v�J�n			�i�J�j
	{ "Character\\TrashBox\\anim\\JumpClose.x",			true,	12.0f	},	// �W�����v��			�i�Ɓj
	{ "Character\\TrashBox\\anim\\JumpOpen.x",			true,	12.0f	},	// �W�����v��			�i�J�j
	{ "Character\\TrashBox\\anim\\JumpEndClose.x",		false,	10.0f	},	// �W�����v�I��			�i�Ɓj
	{ "Character\\TrashBox\\anim\\JumpEndOpen.x",		false,	10.0f	},	// �W�����v�I��			�i�J�j
	{ "Character\\TrashBox\\anim\\AttackStartClose.x",	false,	11.0f	},	// �U���J�n				�i�Ɓj
	{ "Character\\TrashBox\\anim\\AttackStartOpen.x",	false,	10.0f	},	// �U���J�n				�i�J�j
	{ "Character\\TrashBox\\anim\\Attack.x",			false,	5.0f	},	// �U����				�i�J�j
	{ "Character\\TrashBox\\anim\\AttackEnd.x",			false,	24.0f	},	// �U���I��				�i�J�j
	{ "Character\\TrashBox\\anim\\CriticalStartClose.x",false,	11.0f	},	// �N���e�B�J���U���J�n	�i�Ɓj
	{ "Character\\TrashBox\\anim\\CriticalStartOpen.x",	false,	10.0f	},	// �N���e�B�J���U���J�n	�i�J�j
	{ "Character\\TrashBox\\anim\\Critical.x",			false,	5.0f	},	// �N���e�B�J���U����	�i�J�j
	{ "Character\\TrashBox\\anim\\CriticalEnd.x",		false,	24.0f	},	// �N���e�B�J���U���I��	�i�J�j
};

#define PLAYER_HEIGHT 16.0f
#define MOVE_SPEED 0.375f * 2.0f
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f
#define CAPSULE_RADIUS_HOME 2.5f
#define CAPSULE_RADIUS_TRASH 2.5f

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

// �R���X�g���N�^
 CTrashPlayer::CTrashPlayer()
	 : CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	 , mState(EState::eIdle)
	 , IsOpen(false)
	 , mTest(0)
{
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("TrashPlayer");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdleClose);
}

CTrashPlayer::~CTrashPlayer()
{
}

CTrashPlayer* CTrashPlayer::Instance()
{
	return spInstance;
}

// �X�V
void CTrashPlayer::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eMove:
		UpdateMove();
		break;
	case EState::eDamageStart:
		UpdateDamageStart();
		break;
	case EState::eDamage:
		UpdateDamage();
		break;
	case EState::eDamageEnd:
		UpdateDamageEnd();
		break;
	case EState::eJumpStart:
		UpdateJumpStart();
		break;
	case EState::eJump:
		UpdateJump();
		break;
	case EState::eJumpEnd:
		UpdateJumpEnd();
		break;
	case EState::eAttackStart:
		UpdateAttackStart();
		break;
	case EState::eAttack:
		UpdateAttack();
		break;
	case EState::eAttackEnd:
		UpdateAttackEnd();
		break;
	case EState::eCriticalStart:
		UpdateCriticalStart();
		break;
	case EState::eCritical:
		UpdateCritical();
		break;
	case EState::eCriticalEnd:
		UpdateCriticalEnd();
		break;
	}

	//// �ҋ@���ƃW�����v���́A�ړ��������s��
	//if (mState == EState::eIdle
	//	|| mState == EState::eJump)
	//{
	//	UpdateMove();
	//}
	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	CDebugPrint::Print("TState:%d\n", mState);
}

// �`��
void CTrashPlayer::Render()
{
	CXCharacter::Render();
}

// �ҋ@���
void CTrashPlayer::UpdateIdle()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eIdleClose);
	else
		ChangeAnimation(EAnimType::eIdleOpen);
	mTest++;
	if (mTest > 5)
	{
		mTest = 1;
		IsOpen = !IsOpen;
	}

	if (mTest == 1)
		mState = EState::eDamageStart;
	else if (mTest == 2)
		mState = EState::eJumpStart;
	else if (mTest == 3)
		mState = EState::eAttackStart;
	else if (mTest == 4)
		mState = EState::eCriticalStart;
	else if (mTest == 5)
		mState = EState::eMove;
}

// �ړ�
void CTrashPlayer::UpdateMove()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eMoveClose);
	else
		ChangeAnimation(EAnimType::eMoveOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// ��e�J�n
void CTrashPlayer::UpdateDamageStart()
{

	if (!IsOpen)
		ChangeAnimation(EAnimType::eDamageStartClose);
	else
		ChangeAnimation(EAnimType::eDamageStartOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eDamage;
	}
}

// ��e�m�b�N�o�b�N
void CTrashPlayer::UpdateDamage()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eDamageClose);
	else
		ChangeAnimation(EAnimType::eDamageOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eDamageEnd;
	}
}

// ��e�I��
void CTrashPlayer::UpdateDamageEnd()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eDamageEndClose);
	else
		ChangeAnimation(EAnimType::eDamageEndOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �W�����v�J�n
void CTrashPlayer::UpdateJumpStart()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eJumpStartClose);
	else
		ChangeAnimation(EAnimType::eJumpStartOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eJump;
	}
}

// �W�����v��
void CTrashPlayer::UpdateJump()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eJumpClose);
	else
		ChangeAnimation(EAnimType::eJumpOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eJumpEnd;
	}
}

// �W�����v�I��
void CTrashPlayer::UpdateJumpEnd()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eJumpEndClose);
	else
		ChangeAnimation(EAnimType::eJumpEndOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �U���J�n
void CTrashPlayer::UpdateAttackStart()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eAttackStartClose);
	else
		ChangeAnimation(EAnimType::eAttackStartOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eAttack;
	}
}

// �U����
void CTrashPlayer::UpdateAttack()
{
	ChangeAnimation(EAnimType::eAttack);

	if (IsAnimationFinished())
	{
		mState = EState::eAttackEnd;
	}
}

// �U���I��
void CTrashPlayer::UpdateAttackEnd()
{
	ChangeAnimation(EAnimType::eAttackEnd);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �N���e�B�J���U���J�n
void CTrashPlayer::UpdateCriticalStart()
{
	if (!IsOpen)
		ChangeAnimation(EAnimType::eCriticalStartClose);
	else
		ChangeAnimation(EAnimType::eCriticalStartOpen);

	if (IsAnimationFinished())
	{
		mState = EState::eCritical;
	}
}

// �N���e�B�J���U����
void CTrashPlayer::UpdateCritical()
{
	ChangeAnimation(EAnimType::eCritical);

	if (IsAnimationFinished())
	{
		mState = EState::eCriticalEnd;
	}
}

// �N���e�B�J���U���I��
void CTrashPlayer::UpdateCriticalEnd()
{
	ChangeAnimation(EAnimType::eCriticalEnd);

	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �A�j���[�V�����؂�ւ�
void CTrashPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}
