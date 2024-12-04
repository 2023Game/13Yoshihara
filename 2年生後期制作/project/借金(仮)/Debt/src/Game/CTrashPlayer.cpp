//�v���C���[�N���X�̃C���N���[�h
#include "CTrashPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CJobStatusManager.h"

/*
 �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
 (��)���W�����Ă����Ԃ���̃A�j���[�V����
 (�J)���W���J���Ă����Ԃ���̃A�j���[�V����
 (�J��)�������̏�Ԍ��p�̃A�j���[�V����
*/
const CTrashPlayer::AnimData CTrashPlayer::ANIM_DATA[] =
{
	{ "",													true,	0.0f	},	// T�|�[�Y
	{ "Character\\TrashBox\\anim\\Idle_Close.x",			true,	30.0f	},	// �ҋ@					�i�Ɓj
	{ "Character\\TrashBox\\anim\\Idle_Open.x",				true,	30.0f	},	// �ҋ@					�i�J�j
	{ "Character\\TrashBox\\anim\\Move_Close.x",			true,	80.0f	},	// �ړ�					�i�Ɓj
	{ "Character\\TrashBox\\anim\\Move_Open.x",				true,	80.0f	},	// �ړ�					�i�J�j
	{ "Character\\TrashBox\\anim\\Damage_Close_Start.x",	false,	10.0f	},	// ��e�J�n				�i�Ɓj
	{ "Character\\TrashBox\\anim\\Damage_Close.x",			true,	11.0f	},	// ��e�m�b�N�o�b�N		�i�Ɓj
	{ "Character\\TrashBox\\anim\\Damage_Close_End.x",		false,	51.0f	},	// ��e�I��				�i�Ɓj
	{ "Character\\TrashBox\\anim\\Damage_Open_Start.x",		false,	10.0f	},	// ��e�J�n				�i�J�j
	{ "Character\\TrashBox\\anim\\Damage_Open.x",			true,	11.0f	},	// ��e�m�b�N�o�b�N		�i�J�j
	{ "Character\\TrashBox\\anim\\Damage_Open_End.x",		false,	51.0f	},	// ��e�I��				�i�J�j
	{ "Character\\TrashBox\\anim\\Jump_Close_Start.x",		false,	20.0f	},	// �W�����v�J�n			�i�Ɓj
	{ "Character\\TrashBox\\anim\\Jump_Close.x",			true,	11.0f	},	// �W�����v��			�i�Ɓj
	{ "Character\\TrashBox\\anim\\Jump_Close_End.x",		false,	21.0f	},	// �W�����v�I��			�i�Ɓj
	{ "Character\\TrashBox\\anim\\Jump_Open_Start.x",		false,	20.0f	},	// �W�����v�J�n			�i�J�j
	{ "Character\\TrashBox\\anim\\Jump_Open.x",				true,	11.0f	},	// �W�����v��			�i�J�j
	{ "Character\\TrashBox\\anim\\Jump_Open_End.x",			false,	21.0f	},	// �W�����v�I��			�i�J�j
	{ "Character\\TrashBox\\anim\\Attack_Start.x",			false,	35.0f	},	// �U���J�n				�i�J�j
	{ "Character\\TrashBox\\anim\\Attack.x",				false,	11.0f	},	// �U����				�i�J�j
	{ "Character\\TrashBox\\anim\\Attack_End.x",			false,	69.0f	},	// �U���I��				�i�J�j
	{ "Character\\TrashBox\\anim\\Critical_Start.x",		false,	35.0f	},	// �N���e�B�J���U���J�n	�i�J�j
	{ "Character\\TrashBox\\anim\\Critical.x",				false,	11.0f	},	// �N���e�B�J���U����	�i�J�j
	{ "Character\\TrashBox\\anim\\Critical_End.x",			false,	69.0f	},	// �N���e�B�J���U���I��	�i�J�j
	{ "Character\\TrashBox\\anim\\Open.x",					true,	10.0f	},	// �W���J��				�i�Ɓj
	{ "Character\\TrashBox\\anim\\Close.x",					true,	10.0f	},	// �W�����			�i�J�j
};

#define CAPSULE_RADIUS 2.5f
#define PLAYER_HEIGHT 25.0f
#define PLAYER_WIDTH 50.0f

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

// �R���X�g���N�^
CTrashPlayer::CTrashPlayer()
	: CPlayerBase(CAPSULE_RADIUS, PLAYER_HEIGHT)
	, mState(EState::eIdle)
	, mIsOpen(false)
	, mIsJump(false)
	, mTest(0)
{
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

	//�t�B�[���h�Ƃ����Փ˔��������
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(PLAYER_WIDTH - CAPSULE_RADIUS * 10, PLAYER_HEIGHT, 0.0f),
		CVector(-PLAYER_WIDTH + CAPSULE_RADIUS * 10, PLAYER_HEIGHT, 0.0f),
		CAPSULE_RADIUS
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eObject });


	CColliderLine* mpColliderLine = new CColliderLine
	(
		this, ELayer::ePlayer,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle_Close);
}

CTrashPlayer::~CTrashPlayer()
{
}

// �X�V
void CTrashPlayer::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
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
	case EState::eOpenClose:
		UpdateOpenClose();
		break;
	}

	// �ҋ@���ƃW�����v���́A�ړ��������s��
	if (mState == EState::eIdle
		|| mState == EState::eJump)
	{
		UpdateMove();
	}

	// �n�ʂɐڂ��Ă���Ȃ�W�����v���Ă��Ȃ�
	if (mIsGrounded)
		mIsJump = false;

	// �L�����N�^�[�̍X�V
	CPlayerBase::Update();

	CDebugPrint::Print("State:%d\n", mState);
	CDebugPrint::Print("IsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("IsJump:%s\n", mIsJump ? "true" : "false");
}

void CTrashPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}

// �A�N�V�����̃L�[����
void CTrashPlayer::ActionInput()
{
	// �X�y�[�X�ŃW�����v
	if (CInput::PushKey(VK_SPACE))
	{
		mState = EState::eJumpStart;

		// ������Ԃ̃W�����v
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eJump_Close_Start);
		// �J������Ԃ̃W�����v
		else
			ChangeAnimation(EAnimType::eJump_Open_Start);
	}
	// ���N���b�N�ōU��
	if (CInput::PushKey(VK_LBUTTON))
	{
		mMoveSpeed = CVector::zero;
		mState = EState::eAttackStart;

		// ���Ă�����J��
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eOpen);
		else
			ChangeAnimation(EAnimType::eAttack_Start);
	}
	// �E�N���b�N�ŊW�̊J��
	if (CInput::PushKey(VK_RBUTTON))
	{
		mMoveSpeed = CVector::zero;
		mState = EState::eOpenClose;

		// ���Ă�����W���J��
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eOpen);
		// �J���Ă�����W�����
		else
			ChangeAnimation(EAnimType::eClose);
	}
	if (CInput::PushKey('1'))
	{
		mMoveSpeed = CVector::zero;
		mState = EState::eCriticalStart;
		// ���Ă�����J��
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eOpen);
		else
			ChangeAnimation(EAnimType::eCritical_Start);
	}
	if (CInput::PushKey('2'))
	{
		mIsDamage = true;
		mMoveSpeed = CVector::zero;
		SetTakeKnockback(GetStatusBase().mKnockback);
		mState = EState::eDamageStart;
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eDamage_Close_Start);
		else
			ChangeAnimation(EAnimType::eDamage_Open_Start);
	}
}

// �ҋ@���
void CTrashPlayer::UpdateIdle()
{
	if (mIsGrounded)
	{
		ActionInput();
	}
}

// �ړ�
void CTrashPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * GetStatusBase().mMoveSpeed;

		// �W�����v��ԂłȂ���΁A�ړ��A�j���[�V�����ɐ؂�ւ�
		if (mState != EState::eJump)
		{
			mState = EState::eIdle;
			if (!mIsOpen)
				ChangeAnimation(EAnimType::eMove_Close);
			else
				ChangeAnimation(EAnimType::eMove_Open);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			if (!mIsOpen)
				ChangeAnimation(EAnimType::eIdle_Close);
			else
				ChangeAnimation(EAnimType::eIdle_Open);
		}
	}
}

// ��e�J�n
void CTrashPlayer::UpdateDamageStart()
{
	// �A�j���[�V�������I���������e�m�b�N�o�b�N��
	if (IsAnimationFinished())
	{
		// �_���[�W��1�󂯂�
		TakeDamage();
		// �m�b�N�o�b�N���x�̐ݒ�
		mMoveSpeedY = GetStatusBase().mJumpSpeed;
		// �������Ƀm�b�N�o�b�N������
		mMoveSpeed = -VectorZ() * GetTakeKnockback();
		mIsGrounded = false;

		mState = EState::eDamage;

		if (!mIsOpen)
			ChangeAnimation(EAnimType::eDamage_Close);
		else
			ChangeAnimation(EAnimType::eDamage_Open);
	}
}

// ��e�m�b�N�o�b�N
void CTrashPlayer::UpdateDamage()
{
	mIsOpen = true;

	// �n�ʂɕt�������e�I����
	if (mIsGrounded)
	{
		mState = EState::eDamageEnd;

		if (!mIsOpen)
			ChangeAnimation(EAnimType::eDamage_Close_End);
		else
			ChangeAnimation(EAnimType::eDamage_Open_End);
	}
}

// ��e�I��
void CTrashPlayer::UpdateDamageEnd()
{
	// ��e�I�����͈ړ����[��
	mMoveSpeed = CVector::zero;

	// �A�j���[�V������60%�i�s������
	if (GetAnimationFrameRatio() >= 0.60f)
	{
		// ���͉\
		ActionInput();
		// �ړ��\
		UpdateMove();

		mIsDamage = false;
		SetTakeKnockback(0.0f);
	}

	// �A�j���[�V�������I��������ҋ@��
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �W�����v�J�n
void CTrashPlayer::UpdateJumpStart()
{
	// �W�����v�J�n���͈ړ����[��
	mMoveSpeed = CVector::zero;

	// �A�j���[�V�������I��������
	if (IsAnimationFinished())
	{
		// �W�����v���x�̐ݒ�
		mMoveSpeedY = GetStatusBase().mJumpSpeed;
		mIsGrounded = false;

		mState = EState::eJump;
		// ������Ԃ̃W�����v����
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eJump_Close);
		// �J������Ԃ̃W�����v����
		else
			ChangeAnimation(EAnimType::eJump_Open);
	}
}

// �W�����v��
void CTrashPlayer::UpdateJump()
{
	mIsJump = true;
	// �W�����v���͈ړ����Ȃ���J�\
	// �E�N���b�N�ŊW�̊J��
	if (CInput::PushKey(VK_RBUTTON))
	{
		mState = EState::eOpenClose;
		// ���Ă�����W���J��
		if (!mIsOpen)
		{
			ChangeAnimation(EAnimType::eOpen);
		}
		// �J���Ă�����W�����
		else
		{
			ChangeAnimation(EAnimType::eClose);
		}
	}
	// �n�ʂɕt������
	if (mIsGrounded)
	{
		mState = EState::eJumpEnd;
		// ������Ԃ̃W�����v�I����
		if (!mIsOpen)
			ChangeAnimation(EAnimType::eJump_Close_End);
		// �J������Ԃ̃W�����v�I����
		else
			ChangeAnimation(EAnimType::eJump_Open_End);
	}
}

// �W�����v�I��
void CTrashPlayer::UpdateJumpEnd()
{
	// ���n���͈ړ����[��
	mMoveSpeed = CVector::zero;
	// �A�j���[�V������50%�ȏ�i�s������
	if (GetAnimationFrameRatio() >= 0.50f)
	{
		// ���͉\
		ActionInput();
		// �ړ��\
		UpdateMove();
	}

	// �A�j���[�V�������I��������
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// �U���J�n
void CTrashPlayer::UpdateAttackStart()
{
	// �J���Ă��Ȃ���ΊJ���A�j���[�V�����̍Đ������Ă���̂�
	// �I����Ă���U���̍ŏ���
	if (!mIsOpen)
	{
		if (IsAnimationFinished())
		{
			mIsOpen = true;
			ChangeAnimation(EAnimType::eAttack_Start);
		}
	}
	// �J���Ă���Ȃ�U���̍ŏ��̃A�j���[�V�������I�������̂ōU������
	else
	{
		if (IsAnimationFinished())
		{
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eAttack);
		}
	}
}

// �U����
void CTrashPlayer::UpdateAttack()
{
	// �A�j���[�V�������I��������
	if (IsAnimationFinished())
	{
		// �U���I����
		mState = EState::eAttackEnd;
		ChangeAnimation(EAnimType::eAttack_End);
	}
}

// �U���I��
void CTrashPlayer::UpdateAttackEnd()
{
	// �A�j���[�V������52%�ȏ�i�s������
	if (GetAnimationFrameRatio() >= 0.52f)
	{
		// ���͉\
		ActionInput();
		// �ړ��\
		UpdateMove();
	}

	// �A�j���[�V�������I��������ҋ@��
	if (IsAnimationFinished())
	{
		// �Ō�͊W���J�������
		mState = EState::eIdle;
		mIsOpen = true;
	}
}

// �N���e�B�J���U���J�n
void CTrashPlayer::UpdateCriticalStart()
{
	// �J���Ă��Ȃ���ΊJ���A�j���[�V�����̍Đ������Ă���̂�
	// �I����Ă���N���e�B�J���̍ŏ���
	if (!mIsOpen)
	{
		if (IsAnimationFinished())
		{
			mIsOpen = true;
			ChangeAnimation(EAnimType::eCritical_Start);
		}
	}
	// �J���Ă���Ȃ�N���e�B�J���̍ŏ��̃A�j���[�V�������I�������̂ŃN���e�B�J����
	else
	{
		if (IsAnimationFinished())
		{
			mState = EState::eCritical;
			ChangeAnimation(EAnimType::eCritical);
		}
	}
}

// �N���e�B�J���U����
void CTrashPlayer::UpdateCritical()
{
	// �A�j���[�V�������I��������N���e�B�J���I����
	if (IsAnimationFinished())
	{
		mState = EState::eCriticalEnd;
		ChangeAnimation(EAnimType::eCritical_End);
	}
}

// �N���e�B�J���U���I��
void CTrashPlayer::UpdateCriticalEnd()
{
	// �A�j���[�V������52%�ȏ�i�s������
	if (GetAnimationFrameRatio() >= 0.52f)
	{
		// ���͉\
		ActionInput();
		// �ړ��\
		UpdateMove();
	}

	// �A�j���[�V�������I��������ҋ@��
	if (IsAnimationFinished())
	{
		// �Ō�͊W���J�������
		mState = EState::eIdle;
		mIsOpen = true;
	}
}

// �W���J����
void CTrashPlayer::UpdateOpenClose()
{
	if (IsAnimationFinished())
	{
		// �J����Ԃ�ύX
		mIsOpen = !mIsOpen;
		// �W�����v���Ă��Ȃ��Ȃ�ҋ@��
		if (!mIsJump)
		{
			mState = EState::eIdle;
		}
		// �W�����v���Ă���Ȃ�W�����v�֖߂�
		else
		{
			mState = EState::eJump;
			// ������Ԃ̃W�����v
			if (!mIsOpen)
				ChangeAnimation(EAnimType::eJump_Close);
			// �J������Ԃ̃W�����v
			else
				ChangeAnimation(EAnimType::eJump_Open);
		}

	}
}

// �A�j���[�V�����؂�ւ�
void CTrashPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}
