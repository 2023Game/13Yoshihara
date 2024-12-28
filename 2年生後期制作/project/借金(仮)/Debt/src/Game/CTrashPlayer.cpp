#include "CTrashPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"

// �Փˑ���̎ԗ����N���X���擾���邽�߂�
// �ԗ��̊��N���X�̃C���N���[�h
#include "CVehicleBase.h"

// �R���C�_�̃C���N���[�h
#include "CColliderCapsule.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\TrashBox\\anim\\"
/*
 �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
 (��)���W�����Ă����Ԃ���̃A�j���[�V����
 (�J)���W���J���Ă����Ԃ���̃A�j���[�V����
 (�J��)�������̏�Ԍ��p�̃A�j���[�V����
*/
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",								true,	0.0f,	1.0f},	// T�|�[�Y
	{ ANIM_PATH"Idle_Close.x",			true,	30.0f,	1.0f},	// �ҋ@					�i�Ɓj
	{ ANIM_PATH"Idle_Open.x",			true,	30.0f,	1.0f},	// �ҋ@					�i�J�j
	{ ANIM_PATH"Move_Close.x",			true,	80.0f,	1.0f},	// �ړ�					�i�Ɓj
	{ ANIM_PATH"Move_Open.x",			true,	80.0f,	1.0f},	// �ړ�					�i�J�j
	{ ANIM_PATH"Damage_Close_Start.x",	false,	10.0f,	1.0f},	// ��e�J�n				�i�Ɓj
	{ ANIM_PATH"Damage_Close.x",		true,	11.0f,	1.0f},	// ��e�m�b�N�o�b�N		�i�Ɓj
	{ ANIM_PATH"Damage_Close_End.x",	false,	51.0f,	1.0f},	// ��e�I��				�i�Ɓj
	{ ANIM_PATH"Damage_Open_Start.x",	false,	10.0f,	1.0f},	// ��e�J�n				�i�J�j
	{ ANIM_PATH"Damage_Open.x",			true,	11.0f,	1.0f},	// ��e�m�b�N�o�b�N		�i�J�j
	{ ANIM_PATH"Damage_Open_End.x",		false,	51.0f,	1.0f},	// ��e�I��				�i�J�j
	{ ANIM_PATH"Jump_Close_Start.x",	false,	20.0f,	1.0f},	// �W�����v�J�n			�i�Ɓj
	{ ANIM_PATH"Jump_Close.x",			true,	11.0f,	1.0f},	// �W�����v��			�i�Ɓj
	{ ANIM_PATH"Jump_Close_End.x",		false,	21.0f,	1.0f},	// �W�����v�I��			�i�Ɓj
	{ ANIM_PATH"Jump_Open_Start.x",		false,	20.0f,	1.0f},	// �W�����v�J�n			�i�J�j
	{ ANIM_PATH"Jump_Open.x",			true,	11.0f,	1.0f},	// �W�����v��			�i�J�j
	{ ANIM_PATH"Jump_Open_End.x",		false,	21.0f,	1.0f},	// �W�����v�I��			�i�J�j
	{ ANIM_PATH"Attack_Start.x",		false,	35.0f,	1.0f},	// �U���J�n				�i�J�j
	{ ANIM_PATH"Attack.x",				false,	11.0f,	1.0f},	// �U����				�i�J�j
	{ ANIM_PATH"Attack_End.x",			false,	69.0f,	1.0f},	// �U���I��				�i�J�j
	{ ANIM_PATH"Critical_Start.x",		false,	35.0f,	1.0f},	// �N���e�B�J���U���J�n	�i�J�j
	{ ANIM_PATH"Critical.x",			false,	11.0f,	1.0f},	// �N���e�B�J���U����	�i�J�j
	{ ANIM_PATH"Critical_End.x",		false,	69.0f,	1.0f},	// �N���e�B�J���U���I��	�i�J�j
	{ ANIM_PATH"Open.x",				true,	10.0f,	1.0f},	// �W���J��				�i�Ɓj
	{ ANIM_PATH"Close.x",				true,	10.0f,	1.0f},	// �W�����			�i�J�j
};

#define BODY_RADIUS 2.5f	// �{�̂̃R���C�_�\�̔��a
#define BODY_HEIGHT 25.0f	// �{�̂̃R���C�_�\�̍���
#define BODY_WIDTH 50.0f	// �{�̂̃R���C�_�\�̕�

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

// �R���X�g���N�^
CTrashPlayer::CTrashPlayer()
	: CPlayerBase()
	, mState(EState::eIdle)
	, mIsOpen(false)
	, mIsJump(false)
{
	// �A�j���[�V�����ƃ��f���̏�����
	InitAnimationModel("TrashPlayer", &ANIM_DATA);

	// �n�`�A�G�A�U���A�ԗ�
	// �ƏՓ˔��������{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(BODY_WIDTH - BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eEnemy, ETag::eVehicle});
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eObject,
		ELayer::eEnemy, ELayer::eAttackCol, ELayer::eVehicle});

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle_Close);
}

// �f�X�g���N�^
CTrashPlayer::~CTrashPlayer()
{
}

// �X�V
void CTrashPlayer::Update()
{
	// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ�
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::eDamageStart:		UpdateDamageStart();	break;
	case EState::eDamage:			UpdateDamage();			break;
	case EState::eDamageEnd:		UpdateDamageEnd();		break;
	case EState::eJumpStart:		UpdateJumpStart();		break;
	case EState::eJump:				UpdateJump();			break;
	case EState::eJumpEnd:			UpdateJumpEnd();		break;
	case EState::eAttackStart:		UpdateAttackStart();	break;
	case EState::eAttack:			UpdateAttack();			break;
	case EState::eAttackEnd:		UpdateAttackEnd();		break;
	case EState::eCriticalStart:	UpdateCriticalStart();	break;
	case EState::eCritical:			UpdateCritical();		break;
	case EState::eCriticalEnd:		UpdateCriticalEnd();	break;
	case EState::eOpenClose:		UpdateOpenClose();		break;
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

#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("PlayerIsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("PlayerIsJump:%s\n", mIsJump ? "true" : "false");
#endif
}

// �Փˏ���
void CTrashPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CPlayerBase::Collision(self, other, hit);

	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �Փ˂������肪�ԗ��Ȃ�
		if (other->Layer() == ELayer::eVehicle)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);

			// �ԗ��N���X���擾
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			// �ړ����Ȃ琁����΂����
			if (vehicle->IsMove())
			{
				// �ړ����x���[���ɂ���
				mMoveSpeed = CVector::zero;

				// �ԗ��N���X���擾
				CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
				// ���肩�玩���̕���
				CVector direction = Position() - other->Owner()->Position();
				direction.Y(0.0f);
				direction = direction.Normalized();
				// �������󂯂�m�b�N�o�b�N���x�ɁA
				// ���肪�^����m�b�N�o�b�N���x�𑊎肩�玩���̕����ɐݒ�
				SetKnockbackReceived(direction * vehicle->GetKnockbackDealt());

				// �U�����󂯂Ă��Ȃ���Δ�e�J�n�A�j���[�V�����ɕύX
				if (!mIsDamage)
				{
					// �W�����܂��Ă�����W�����܂��Ă����Ԃ���̃A�j���[�V����
					if (!mIsOpen)
						ChangeAnimation((int)EAnimType::eDamage_Close_Start);
					// �W���J���Ă�����W���J���Ă����Ԃ���̃A�j���[�V����
					else
						ChangeAnimation((int)EAnimType::eDamage_Open_Start);
				}

				// �U�����󂯂Ă���
				mIsDamage = true;
				// ��Ԃ��e�J�n�ɕύX
				ChangeState(EState::eDamageStart);
			}
		}
		// �Փ˂������肪�G�Ȃ�
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
}

// �A�N�V�����̃L�[����
void CTrashPlayer::ActionInput()
{
	// �X�y�[�X�ŃW�����v
	if (CInput::PushKey(VK_SPACE))
	{
		ChangeState(EState::eJumpStart);

		// ������Ԃ̃W�����v
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eJump_Close_Start);
		// �J������Ԃ̃W�����v
		else
			ChangeAnimation((int)EAnimType::eJump_Open_Start);
	}
	// ���N���b�N�ōU��
	if (CInput::PushKey(VK_LBUTTON))
	{
		mMoveSpeed = CVector::zero;
		ChangeState(EState::eAttackStart);

		// ���Ă�����J��
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eOpen);
		else
			ChangeAnimation((int)EAnimType::eAttack_Start);
	}
	// �E�N���b�N�ŊW�̊J��
	if (CInput::PushKey(VK_RBUTTON))
	{
		mMoveSpeed = CVector::zero;
		ChangeState(EState::eOpenClose);

		// ���Ă�����W���J��
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eOpen);
		// �J���Ă�����W�����
		else
			ChangeAnimation((int)EAnimType::eClose);
	}
	if (CInput::PushKey('1'))
	{
		mMoveSpeed = CVector::zero;
		ChangeState(EState::eCriticalStart);
		// ���Ă�����J��
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eOpen);
		else
			ChangeAnimation((int)EAnimType::eCritical_Start);
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
		mMoveSpeed += move * GetBaseMoveSpeed();

		// �W�����v��ԂłȂ���΁A�ړ��A�j���[�V�����ɐ؂�ւ�
		if (mState != EState::eJump)
		{
			ChangeState(EState::eIdle);
			if (!mIsOpen)
				ChangeAnimation((int)EAnimType::eMove_Close);
			else
				ChangeAnimation((int)EAnimType::eMove_Open);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			if (!mIsOpen)
				ChangeAnimation((int)EAnimType::eIdle_Close);
			else
				ChangeAnimation((int)EAnimType::eIdle_Open);
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
		TakeDamage(1,nullptr);		
		// �m�b�N�o�b�N���̔�яオ��̑��x��
		// �󂯂�m�b�N�o�b�N���x�̔����ɐݒ�
		mMoveSpeedY = GetKnockbackReceived().Length() * 0.5f;
		// �󂯂�m�b�N�o�b�N���x���ړ��ɐݒ�
		mMoveSpeed = GetKnockbackReceived();
		mIsGrounded = false;

		ChangeState(EState::eDamage);

		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eDamage_Close);
		else
			ChangeAnimation((int)EAnimType::eDamage_Open);
	}
}

// ��e�m�b�N�o�b�N
void CTrashPlayer::UpdateDamage()
{
	mIsOpen = true;

	// �n�ʂɕt�������e�I����
	if (mIsGrounded)
	{
		ChangeState(EState::eDamageEnd);

		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eDamage_Close_End);
		else
			ChangeAnimation((int)EAnimType::eDamage_Open_End);
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
		SetKnockbackReceived(CVector::zero);
	}

	// �A�j���[�V�������I��������ҋ@��
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
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
		mMoveSpeedY = GetJumpSpeed();
		mIsGrounded = false;

		ChangeState(EState::eJump);
		// ������Ԃ̃W�����v����
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eJump_Close);
		// �J������Ԃ̃W�����v����
		else
			ChangeAnimation((int)EAnimType::eJump_Open);
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
		ChangeState(EState::eOpenClose);
		// ���Ă�����W���J��
		if (!mIsOpen)
		{
			ChangeAnimation((int)EAnimType::eOpen);
		}
		// �J���Ă�����W�����
		else
		{
			ChangeAnimation((int)EAnimType::eClose);
		}
	}
	// �n�ʂɕt������
	if (mIsGrounded)
	{
		ChangeState(EState::eJumpEnd);
		// ������Ԃ̃W�����v�I����
		if (!mIsOpen)
			ChangeAnimation((int)EAnimType::eJump_Close_End);
		// �J������Ԃ̃W�����v�I����
		else
			ChangeAnimation((int)EAnimType::eJump_Open_End);
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
		ChangeState(EState::eIdle);
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
			ChangeAnimation((int)EAnimType::eAttack_Start);
		}
	}
	// �J���Ă���Ȃ�U���̍ŏ��̃A�j���[�V�������I�������̂ōU������
	else
	{
		if (IsAnimationFinished())
		{
			ChangeState(EState::eAttack);
			ChangeAnimation((int)EAnimType::eAttack);
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
		ChangeState(EState::eAttackEnd);
		ChangeAnimation((int)EAnimType::eAttack_End);
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
		ChangeState(EState::eIdle);
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
			ChangeAnimation((int)EAnimType::eCritical_Start);
		}
	}
	// �J���Ă���Ȃ�N���e�B�J���̍ŏ��̃A�j���[�V�������I�������̂ŃN���e�B�J����
	else
	{
		if (IsAnimationFinished())
		{
			ChangeState(EState::eCritical);
			ChangeAnimation((int)EAnimType::eCritical);
		}
	}
}

// �N���e�B�J���U����
void CTrashPlayer::UpdateCritical()
{
	// �A�j���[�V�������I��������N���e�B�J���I����
	if (IsAnimationFinished())
	{
		ChangeState(EState::eCriticalEnd);
		ChangeAnimation((int)EAnimType::eCritical_End);
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
		ChangeState(EState::eIdle);
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
			ChangeState(EState::eIdle);
		}
		// �W�����v���Ă���Ȃ�W�����v�֖߂�
		else
		{
			ChangeState(EState::eJump);
			// ������Ԃ̃W�����v
			if (!mIsOpen)
				ChangeAnimation((int)EAnimType::eJump_Close);
			// �J������Ԃ̃W�����v
			else
				ChangeAnimation((int)EAnimType::eJump_Open);
		}

	}
}

// ��Ԑ؂�ւ�
void CTrashPlayer::ChangeState(EState state)
{
	// ���ɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	mState = state;
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CTrashPlayer::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "�ҋ@";
	case EState::eDamageStart:		return "��e�J�n";
	case EState::eDamage:			return "��e��";
	case EState::eDamageEnd:		return "��e�I��";
	case EState::eJumpStart:		return "�W�����v�J�n";
	case EState::eJump:				return "�W�����v��";
	case EState::eJumpEnd:			return "�W�����v�I��";
	case EState::eAttackStart:		return "�U���J�n";
	case EState::eAttack:			return "�U����";
	case EState::eAttackEnd:		return "�U���I��";
	case EState::eCriticalStart:	return "�N���e�B�J���U���J�n";
	case EState::eCritical:			return "�N���e�B�J���U����";
	case EState::eCriticalEnd:		return "�N���e�B�J���U���I��";
	case EState::eOpenClose:		return "�J��";
	}
	return "";
}
#endif
