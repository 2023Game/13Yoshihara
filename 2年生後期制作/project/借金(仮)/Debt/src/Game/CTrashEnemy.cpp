#include "CTrashEnemy.h"
#include "CDebugFieldOfView.h"
#include "CPlayerBase.h"
#include "Primitive.h"

// TODO�F��ŏ����e�X�g�p
#include "CInput.h"

// �Փˑ���̎ԗ����N���X���擾���邽�߂�
// �ԗ��̊��N���X�̃C���N���[�h
#include "CVehicleBase.h"

// �R���C�_�̃C���N���[�h
#include "CColliderCapsule.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\TrashBox\\anim\\"
/*
 �G�̃A�j���[�V�����f�[�^�̃e�[�u��
 (��)���W�����Ă����Ԃ���̃A�j���[�V����
 (�J)���W���J���Ă����Ԃ���̃A�j���[�V����
 (�J��)�������̏�Ԍ��p�̃A�j���[�V����
*/
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
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

#define FOV_ANGLE 45.0f		// ����͈͂̊p�x
#define FOV_LENGTH 100.0f	// ����͈͂̋���

#define PATROL_INTERVAL	3.0f// ���̏���|�C���g�Ɉړ��J�n����܂ł̎���
#define IDLE_TIME 5.0f		// �ҋ@��Ԃ̎���

#define PATROL_POS0 CVector(50.0f,0.0f,0.0f)
#define PATROL_POS1 CVector(0.0f,0.0f,0.0f)
#define PATROL_POS2 CVector(0.0f,0.0f,100.0f)
#define PATROL_POS3 CVector(50.0f,0.0f,100.0f)


// �R���X�g���N�^
CTrashEnemy::CTrashEnemy()
	: CEnemyBase
	(
		FOV_ANGLE,
		FOV_LENGTH,
		{ 
			PATROL_POS0,
			PATROL_POS1,
			PATROL_POS2,
			PATROL_POS3
		}
	)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsOpen(false)
	, mIsJump(false)
{
	// �A�j���[�V�����ƃ��f���̏�����
	InitAnimationModel("TrashEnemy", &ANIM_DATA);

	// �n�`�A�v���C���[�A�G�A�U���A�ԗ�
	// �ƏՓ˔��������{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(BODY_WIDTH - BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eObject,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eAttackCol, ELayer::eVehicle});

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle_Close);
}

// �f�X�g���N�^
CTrashEnemy::~CTrashEnemy()
{
}

// �X�V
void CTrashEnemy::Update()
{
	if (CInput::PushKey('U'))
	{
		ChangeState(EState::eChase);
	}
	if (CInput::PushKey('L'))
	{
		ChangeState(EState::ePatrol);
	}

	// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ�
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::ePatrol:			UpdatePatrol();			break;
	case EState::eChase:			UpdateChase();			break;
	case EState::eLost:				UpdateLost();			break;
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


	// �n�ʂɐڂ��Ă���Ȃ�W�����v���Ă��Ȃ�
	if (mIsGrounded)
		mIsJump = false;

	// �L�����N�^�[�̍X�V
	CEnemyBase::Update();

#if _DEBUG
	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("EnemyIsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("EnemyIsJump:%s\n", mIsJump ? "true" : "false");
#endif
}

// �Փˏ���
void CTrashEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ���N���X�̏Փˏ���
	CEnemyBase::Collision(self, other, hit);

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
				direction = direction.Normalized();
				direction.Y(0.0f);
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
		// �Փ˂������肪�v���C���[�Ȃ�
		else if (other->Layer() == ELayer::ePlayer)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
}

// �`��
void CTrashEnemy::Render()
{
	CEnemyBase::Render();

	// ����������Ԃł���΁A
	if (mState == EState::eLost)
	{
		// �v���C���[�����������ʒu�Ƀf�o�b�O�\��
		float rad = 2.0f;
		CMatrix m;
		m.Translate(mLostPlayerPos + CVector(0.0f, rad, 0.0f));
		Primitive::DrawWireSphere(m, rad, CColor::blue);
	}
}

// �ҋ@���
void CTrashEnemy::UpdateIdle()
{
	// �v���C���[������͈͓��ɓ�������A�ǐՏ�Ԃֈڍs
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// �ҋ@�A�j���[�V�������Đ�
	if (!mIsOpen)
	{
		ChangeAnimation((int)EAnimType::eIdle_Close);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eIdle_Open);
	}

	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// �ҋ@���Ԃ��o�߂�����A�����Ԃֈڍs
		ChangeState(EState::ePatrol);
		mElapsedTime = 0.0f;
	}
}

// ���񏈗�
void CTrashEnemy::UpdatePatrol()
{
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
	// �X�e�b�v0�F����J�n���̏���|�C���g�����߂�
	case 0:
		mNextPatrolIndex = -1;
		ChangePatrolPoint();
		mStateStep++;
		break;
	// �X�e�b�v1�F����|�C���g�܂ňړ�
	case 1:
		// �ړ��A�j���[�V�������Đ�
		if (!mIsOpen)
		{
			ChangeAnimation((int)EAnimType::eMove_Close);
		}
		else
		{
			ChangeAnimation((int)EAnimType::eMove_Open);
		}

		if (MoveTo(mPatrolPoints[mNextPatrolIndex], GetBaseMoveSpeed()))
		{
			mStateStep++;
		}
		break;
	// �X�e�b�v2�F�ړ���̑ҋ@
	case 2:
		// �ҋ@�A�j���[�V�������Đ�
		if (!mIsOpen)
		{
			ChangeAnimation((int)EAnimType::eIdle_Close);
		}
		else
		{
			ChangeAnimation((int)EAnimType::eIdle_Open);
		}

		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangePatrolPoint();
			mStateStep = 1;
			mElapsedTime = 0.0f;
		}
	}
}

// �ǐՏ���
void CTrashEnemy::UpdateChase()
{
	// �v���C���[������͈͊O�ɏo����A����������Ԃֈڍs
	if (!IsFoundPlayer())
	{
		ChangeState(EState::eLost);
		return;
	}
	// �v���C���[�ɍU���ł���Ȃ�΁A�U����Ԃֈڍs
	if (CanAttackPlayer())
	{
		ChangeState(EState::eAttackStart);

		// ���Ă�����J���A�j���[�V�������Đ�
		if (!mIsOpen)
		{
			ChangeAnimation((int)EAnimType::eOpen);
		}
		// �J���Ă����炷���U���A�j���[�V�������Đ�
		else
		{
			ChangeAnimation((int)EAnimType::eAttack_Start);
		}
		return;
	}

	// �ړ��A�j���[�V�������Đ�
	if (!mIsOpen)
	{
		ChangeAnimation((int)EAnimType::eMove_Close);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eMove_Open);
	}

	// �v���C���[�̍��W�֌����Ĉړ�����
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerPos = player->Position();
	mLostPlayerPos = playerPos;	// �v���C���[���Ō�Ɍ������W���X�V
	if (MoveTo(playerPos, GetBaseMoveSpeed()))
	{

	}
}

// ����������
void CTrashEnemy::UpdateLost()
{
	// �v���C���[������͈͓��ɓ�������A�ǐՏ�Ԃֈڍs
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// �ړ��A�j���[�V�������Đ�
	if (!mIsOpen)
	{
		ChangeAnimation((int)EAnimType::eMove_Close);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eMove_Open);
	}

	// �v���C���[�����������ʒu�܂ňړ�
	if (MoveTo(mLostPlayerPos, GetBaseMoveSpeed()))
	{
		// �ړ����I���΁A�ҋ@��Ԃֈڍs
		ChangeState(EState::eIdle);
	}
}

// ��e�J�n
void CTrashEnemy::UpdateDamageStart()
{
	// �A�j���[�V�������I���������e�m�b�N�o�b�N��
	if (IsAnimationFinished())
	{
		// �_���[�W��1�󂯂�
		TakeDamage(1, nullptr);
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
void CTrashEnemy::UpdateDamage()
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
void CTrashEnemy::UpdateDamageEnd()
{
	// ��e�I�����͈ړ����[��
	mMoveSpeed = CVector::zero;

	// �A�j���[�V������60%�i�s������
	if (GetAnimationFrameRatio() >= 0.60f)
	{
		// �ړ��\

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
void CTrashEnemy::UpdateJumpStart()
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

void CTrashEnemy::UpdateJump()
{

}

void CTrashEnemy::UpdateJumpEnd()
{

}

// �U���J�n
void CTrashEnemy::UpdateAttackStart()
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
void CTrashEnemy::UpdateAttack()
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
void CTrashEnemy::UpdateAttackEnd()
{
	// �A�j���[�V�������I��������ҋ@��
	if (IsAnimationFinished())
	{
		// �Ō�͊W���J�������
		ChangeState(EState::eIdle);
		mIsOpen = true;
	}
}

void CTrashEnemy::UpdateCriticalStart()
{
}

void CTrashEnemy::UpdateCritical()
{
}

void CTrashEnemy::UpdateCriticalEnd()
{
}

void CTrashEnemy::UpdateOpenClose()
{
}

// ��Ԑ؂�ւ�
void CTrashEnemy::ChangeState(EState state)
{
	// ���ɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	mState = state;
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CTrashEnemy::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "�ҋ@";
	case EState::ePatrol:			return "����";
	case EState::eChase:			return "�ǐ�";
	case EState::eLost:				return "������";
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

// ��Ԃ̐F���擾
CColor CTrashEnemy::GetStateColor(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return CColor::white;
	case EState::ePatrol:			return CColor::green;
	case EState::eChase:			return CColor::red;
	case EState::eLost:				return CColor::yellow;
	case EState::eDamageStart:		return CColor::blue;
	case EState::eDamage:			return CColor::blue;
	case EState::eDamageEnd:		return CColor::blue;
	case EState::eJumpStart:		return CColor::gray;
	case EState::eJump:				return CColor::gray;
	case EState::eJumpEnd:			return CColor::gray;
	case EState::eAttackStart:		return CColor::magenta;
	case EState::eAttack:			return CColor::magenta;
	case EState::eAttackEnd:		return CColor::magenta;
	case EState::eCriticalStart:	return CColor::black;
	case EState::eCritical:			return CColor::black;
	case EState::eCriticalEnd:		return CColor::black;
	case EState::eOpenClose:		return CColor::white;
	}
	return CColor::white;
}

#endif