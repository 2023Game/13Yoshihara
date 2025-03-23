#include "CPlayer.h"
#include "CColliderCapsule.h"
#include "CInput.h"

#define BODY_RADIUS 2.5f
#define BODY_HEIGHT 12.5f

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Player\\anim\\"

// �A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f},	// T�|�[�Y
	{ ANIM_PATH"idle.x",		true,	153.0f,	1.0f	},	// �ҋ@
	{ ANIM_PATH"walk.x",		true,	66.0f,	1.0f	},	// ���s
	{ ANIM_PATH"attack.x",		false,	92.0f,	1.0f	},	// �a��U��
	{ ANIM_PATH"kick.x",		false,	74.0f,	1.0f	},	// �R��U��
	{ ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// �W�����v�J�n
	{ ANIM_PATH"jump.x",		true,	1.0f,	1.0f	},	// �W�����v��
	{ ANIM_PATH"jump_end.x",	false,	26.0f,	1.0f	},	// �W�����v�I��
	{ ANIM_PATH"hit.x",			false,	44.0f,	1.0f	},	// ������
	//{ ANIM_PATH"Jump.x",		true,	0.0f,	1.0f},	// �W�����v
	//// �U��
	//{ ANIM_PATH"AttackW_1.x",	false,	0.0f,	1.0f},	// W
	//{ ANIM_PATH"AttackA_1.x",	false,	0.0f,	1.0f},	// A
	//{ ANIM_PATH"AttackS_1.x",	false,	0.0f,	1.0f},	// S
	//{ ANIM_PATH"AttackD_1.x",	false,	0.0f,	1.0f},	// D
	//// ��e
	//{ ANIM_PATH"DamageW_1.x",	false,	0.0f,	1.0f},	// W
	//{ ANIM_PATH"DamageA_1.x",	false,	0.0f,	1.0f},	// A
	//{ ANIM_PATH"DamageS_1.x",	false,	0.0f,	1.0f},	// S
	//{ ANIM_PATH"DamageD_1.x",	false,	0.0f,	1.0f},	// D
	//// ���
	//{ ANIM_PATH"DodgeW_1.x",	false,	0.0f,	1.0f},	// W
	//{ ANIM_PATH"DodgeA_1.x",	false,	0.0f,	1.0f},	// A
	//{ ANIM_PATH"DodgeS_1.x",	false,	0.0f,	1.0f},	// S
	//{ ANIM_PATH"DodgeD_1.x",	false,	0.0f,	1.0f},	// D
	//// �p���B
	//{ ANIM_PATH"ParryW_1.x",	false,	0.0f,	1.0f},	// W
	//{ ANIM_PATH"ParryA_1.x",	false,	0.0f,	1.0f},	// A
	//{ ANIM_PATH"ParryS_1.x",	false,	0.0f,	1.0f},	// S
	//{ ANIM_PATH"ParryD_1.x",	false,	0.0f,	1.0f},	// D
};

// �R���X�g���N�^
CPlayer::CPlayer()
	: CPlayerBase()
	, CPlayerStatus()
	, mState(EState::eIdle)
{
	// �A�j���[�V�����ƃ��f���̏�����
	InitAnimationModel("Player", &ANIM_DATA);
	
	// �R���C�_�\�𐶐�
	CreateCol();

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{
}

// �U������
bool CPlayer::IsAttacking() const
{
	return false;
}

// �U���J�n
void CPlayer::AttackStart()
{
}

// �U���I��
void CPlayer::AttackEnd()
{
}

// �_���[�W���󂯂�
void CPlayer::TakeDamage(int damage, CObjectBase* causer)
{
}

// �X�V
void CPlayer::Update()
{
	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;
	case EState::eDamageStart:	UpdateDamageStart();	break;
	case EState::eDamage:		UpdateDamage();			break;
	case EState::eDamageEnd:	UpdateDamageEnd();		break;
	case EState::eAttackStart:	UpdateAttackStart();	break;
	case EState::eAttack:		UpdateAttack();			break;
	case EState::eAttackEnd:	UpdateAttackEnd();		break;
	case EState::eDodgeStart:	UpdateDodgeStart();		break;
	case EState::eDodge:		UpdateDodge();			break;
	case EState::eDodgeEnd:		UpdateDamageEnd();		break;
	case EState::eParryStart:	UpdateParryStart();		break;
	case EState::eParry:		UpdateParry();			break;
	case EState::eParryEnd:		UpdateParryEnd();		break;
	case EState::eDeath:		UpdateDeath();			break;
	}

	// �ҋ@���́A�ړ��������s��
	if (mState == EState::eIdle)
	{
		UpdateMove();
	}

	CPlayerBase::Update();

#if _DEBUG
	CDebugPrint::Print("PlayerState:%d\n", GetStateStr(mState));
#endif
}

// �Փˏ���
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}

// �R���C�_�\�𐶐�
void CPlayer::CreateCol()
{
	// �{�̃R���C�_
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	// �t�B�[���h,�ǁA�I�u�W�F�N�g�Ƃ����Փ�
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject });
}

// �A�N�V�����̃L�[����
void CPlayer::ActionInput()
{
	// �ڒn���Ă����
	if (mIsGrounded)
	{
		if (CInput::PushKey(VK_LBUTTON))
		{

		}
	}
}

// �ҋ@���
void CPlayer::UpdateIdle()
{
	// �L�[����
	ActionInput();
}

// �ړ�����
void CPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;
	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * GetBaseMoveSpeed() * Times::DeltaTime();

		// �ҋ@��Ԃł���΁A�ړ��A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{	
			// �u�����h���I�����Ă�����
			if (ChangeAnimationBlend((int)EAnimType::eMove, 5.0f))
			{
				// �ړ��A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation((int)EAnimType::eMove);
			}
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			// �u�����h���I�����Ă�����
			if (ChangeAnimationBlend((int)EAnimType::eIdle, 5.0f))
			{
				// �ҋ@�A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation((int)EAnimType::eIdle);
			}
		}
	}
}

// ��e�J�n
void CPlayer::UpdateDamageStart()
{
}

// ��e�m�b�N�o�b�N
void CPlayer::UpdateDamage()
{
}

// ��e�I��
void CPlayer::UpdateDamageEnd()
{
}

// �W�����v�J�n
void CPlayer::UpdateJumpStart()
{
}

// �W�����v��
void CPlayer::UpdateJump()
{
}

// �W�����v�I��
void CPlayer::UpdateJumpEnd()
{
}

// �U���J�n
void CPlayer::UpdateAttackStart()
{
}

// �U����
void CPlayer::UpdateAttack()
{
}

// �U���I��
void CPlayer::UpdateAttackEnd()
{
}

// ����J�n
void CPlayer::UpdateDodgeStart()
{
}

// ���
void CPlayer::UpdateDodge()
{
}

// ����I��
void CPlayer::UpdateDodgeEnd()
{
}

// �p���B�J�n
void CPlayer::UpdateParryStart()
{
}

// �p���B��
void CPlayer::UpdateParry()
{
}

// �p���B�I��
void CPlayer::UpdateParryEnd()
{
}

// ���S�̍X�V����
void CPlayer::UpdateDeath()
{
}

// ���S
void CPlayer::Death()
{
}

// ��Ԑ؂�ւ�
void CPlayer::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
}

#if _DEBUG

// ��Ԃ̕�������擾
std::string CPlayer::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:			return "�ҋ@";			break;
	case EState::eMove:			return "�ړ�";			break;
	case EState::eDamageStart:	return "��e�J�n";		break;
	case EState::eDamage:		return "��e��";		break;
	case EState::eDamageEnd:	return "��e�I��";		break;
	case EState::eJumpStart:	return "�W�����v�J�n";	break;
	case EState::eJump:			return "�W�����v��";	break;
	case EState::eJumpEnd:		return "�W�����v�I��";	break;
	case EState::eAttackStart:	return "�U���J�n";		break;
	case EState::eAttack:		return "�U����";		break;
	case EState::eAttackEnd:	return "�U���I��";		break;
	case EState::eDodgeStart:	return "����J�n";		break;
	case EState::eDodge:		return "���";		break;
	case EState::eDodgeEnd:		return "����I��";		break;
	case EState::eParryStart:	return "�p���B�J�n";	break;
	case EState::eParry:		return "�p���B��";		break;
	case EState::eParryEnd:		return "�p���B�I��";	break;
	case EState::eDeath:		return "���S";			break;
	}
}

#endif
