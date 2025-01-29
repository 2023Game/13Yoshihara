#include "CTrashPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CTrashEnemy.h"
#include "CGaugeUI2D.h"
#include "CTrashBag.h"
#include "CTrashField.h"

// �Փˑ���̃N���X���擾���邽�߂̃C���N���[�h
#include "CVehicleBase.h"
#include "CCollector.h"

// �R���C�_�̃C���N���[�h
#include "CColliderCapsule.h"
#include "CColliderSphere.h"

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
	{ ANIM_PATH"Damage_Open_Start.x",	false,	10.0f,	1.0f},	// ��e�J�n				�i�J�j
	{ ANIM_PATH"Damage.x",				true,	11.0f,	1.0f},	// ��e�m�b�N�o�b�N		�i�J�j
	{ ANIM_PATH"Damage_End.x",			false,	51.0f,	1.0f},	// ��e�I��				�i�J�j
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
	{ ANIM_PATH"Open.x",				false,	10.0f,	1.0f},	// �W���J��				�i�Ɓj
	{ ANIM_PATH"Close.x",				false,	10.0f,	1.0f},	// �W�����			�i�J�j
	{ ANIM_PATH"Death.x",				false,   5.0f,  1.0f},	// ���S					�i�J�j
};

#define BODY_RADIUS 2.5f	// �{�̂̃R���C�_�\�̔��a
#define BODY_HEIGHT 25.0f	// �{�̂̃R���C�_�\�̍���
#define BODY_WIDTH 50.0f	// �{�̂̃R���C�_�\�̕�

// �U���R���C�_�\
#define ATTACK_COL_RADIUS	2.0f	// ���a
#define ATTACK_COL_HEIGHT	25.0f	// ����
#define ATTACK_COL_WIDTH	70.0f	// ��
// �I�t�Z�b�g���W
#define ATTACK_COL_OFFSET_POS CVector(0.0f,0.0f,100.0f)

// �N���e�B�J���U���R���C�_�[
#define CRITICAL_COL_RADIUS		3.0f		// ���a
#define CRITICAL_COL_HEIGHT		25.0f		// ����
#define CRITICAL_COL_WIDTH		140.0f		// ��
// �I�t�Z�b�g���W
#define CRITICAL_COL_OFFSET_POS CVector(0.0f,0.0f,160.0f)

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

#define SCALE 0.1f	// �X�P�[��

// Hp�Q�[�W�̉摜�̃p�X
#define HP_GAUGE_PATH "UI\\trashbox_player_hp_gauge.png"
#define HP_GAUGE_SIZE 1.5f	// Hp�Q�[�W�̑傫���̔{��
#define HP_GAUGE_OFFSET_POS CVector(10.0f,10.0f,0.0f)

// �S�~�܂𗎂Ƃ��I�t�Z�b�g���W
#define TRASH_BAG_OFFSET_POS CVector(0.0f,5.0f,0.0f)

// �R���X�g���N�^
CTrashPlayer::CTrashPlayer()
	: CPlayerBase()
	, CTrashPlayerStatus()
	, mState(EState::eIdle)
	, mIsOpen(false)
	, mIsJump(false)
	, mIsStickCollector(false)
	, mpStickCollector(nullptr)
{
	// Hp�Q�[�W��ݒ�
	mpHpGauge = new CGaugeUI2D(this, HP_GAUGE_PATH);
	mpHpGauge->Position(HP_GAUGE_OFFSET_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());
	mpHpGauge->Size(HP_GAUGE_SIZE);
	// �傫���̒���
	Scale(SCALE, SCALE, SCALE);
	// �A�j���[�V�����ƃ��f���̏�����
	InitAnimationModel("TrashPlayer", &ANIM_DATA);

	// �{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector( BODY_WIDTH - BODY_RADIUS / SCALE, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS / SCALE, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	// �n�`�A�G�A������A�U���A�ԗ��A�L�����T�m�p�A�S�~��
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eEnemy, ETag::eVehicle,ETag::eTrashBag});
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,
		ELayer::eEnemy, ELayer::eCollector, ELayer::eAttackCol, ELayer::eVehicle,
		ELayer::eCharaSearch,ELayer::eTrashBag});

	// �U���R���C�_�[
	mpAttackCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, ATTACK_COL_HEIGHT, ATTACK_COL_WIDTH - ATTACK_COL_RADIUS * 10),
		CVector(0.0f, ATTACK_COL_HEIGHT, -ATTACK_COL_WIDTH + ATTACK_COL_RADIUS * 10),
		ATTACK_COL_RADIUS
	);
	// �N���e�B�J���U���R���C�_�[
	mpCriticalCol = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, CRITICAL_COL_HEIGHT, CRITICAL_COL_WIDTH - CRITICAL_COL_RADIUS * 10),
		CVector(0.0f, CRITICAL_COL_HEIGHT, -CRITICAL_COL_WIDTH + CRITICAL_COL_RADIUS * 10),
		CRITICAL_COL_RADIUS
	);

	// �G�Ɖ�����Ǝԗ��ƏՓ˔��肷��
	mpAttackCol->SetCollisionTags({ ETag::eEnemy,ETag::eVehicle });
	mpAttackCol->SetCollisionLayers({ ELayer::eEnemy,ELayer::eCollector,ELayer::eVehicle });
	mpCriticalCol->SetCollisionTags({ ETag::eEnemy,ETag::eVehicle });
	mpCriticalCol->SetCollisionLayers({ ELayer::eEnemy,ELayer::eCollector,ELayer::eVehicle });
	
	// �����̑O�Ɉʒu����
	mpAttackCol->Position(ATTACK_COL_OFFSET_POS);
	mpCriticalCol->Position(CRITICAL_COL_OFFSET_POS);

	// �U���R���C�_�[�͍ŏ��̓I�t�ɂ��Ă���
	mpAttackCol->SetEnable(false);
	mpCriticalCol->SetEnable(false);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle_Close);
}

// �f�X�g���N�^
CTrashPlayer::~CTrashPlayer()
{
	SAFE_DELETE(mpCriticalCol);
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
	case EState::eDeath:			UpdateDeath();			break;
	}

	// �ҋ@���ƃW�����v����
	// �U���J�n�I���A�N���e�B�J���J�n�I���́A�ړ��������s��
	if (mState == EState::eIdle ||
		mIsJump ||
		mState == EState::eAttackStart ||
		mState == EState::eAttackEnd||
		mState==EState::eCriticalStart||
		mState == EState::eCriticalEnd)
	{
		UpdateMove();
	}

	// �L�����N�^�[�̍X�V
	CPlayerBase::Update();

	// HP�Q�[�W���X�V
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());

#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("PlayerIsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("PlayerIsJump:%s\n", mIsJump ? "true" : "false");
	CDebugPrint::Print("PlayerHp:%d\n", GetHp());
	CDebugPrint::Print("PlayerIsCollector:%s\n", mIsStickCollector ? "true" : "false");
	CDebugPrint::Print("PlayerTrashBagNum:%d\n", GetTrashBag());
	CDebugPrint::Print("PlayerGoldBagNum:%d\n", GetGoldTrashBag());
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
		// �Փ˂������肪������Ȃ�
		else if (other->Layer() == ELayer::eCollector)
		{
			// ������̃N���X���擾
			CCollector* collector = dynamic_cast<CCollector*>(other->Owner());

			// �U�����łȂ��Ȃ�
			if (!collector->IsAttacking())
			{
				// �����߂��x�N�g��
				CVector adjust = hit.adjust;
				adjust.Y(0.0f);

				// �����߂��x�N�g���̕��A���W���ړ�
				Position(Position() + adjust * hit.weight);
			}
			// �U�����Ȃ�
			else if (collector->IsAttacking())
			{
				// �W���J���Ă��Ȃ��ꍇ
				if (!mIsOpen)
				{
					// �W���J��
					ChangeState(EState::eOpenClose);
				}
			}
		}
	}
	// �U���R���C�_�[
	else if (self == mpAttackCol)
	{
		// �Փ˂������肪�G�Ȃ�
		if (other->Layer() == ELayer::eEnemy)
		{
			// �G�N���X���擾
			CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());

			if (enemy != nullptr &&
				!IsAttackHitObj(enemy))
			{
				AddAttackHitObj(enemy);
				// �������瑊��̕���
				CVector direction = enemy->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
				// �������^����m�b�N�o�b�N���x���������瑊��̕����ɐݒ�
				enemy->SetKnockbackReceived(direction * GetKnockbackDealt());
				// �U���͕��̃_���[�W��^����
				enemy->TakeDamage(GetAttackPower(), this, GetPower());
			}
		}
		// �Փ˂������肪������Ȃ�
		else if (other->Layer() == ELayer::eCollector)
		{
			// ������N���X���擾
			CCollector* collector = dynamic_cast<CCollector*>(other->Owner());

			if (collector != nullptr &&
				!IsAttackHitObj(collector))
			{
				AddAttackHitObj(collector);
				// �U���͕��̃_���[�W��^����
				collector->TakeDamage(GetAttackPower(), this);
			}
		}
		// �Փ˂������肪�ԗ��Ȃ�
		else if (other->Layer() == ELayer::eVehicle)
		{
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			if (vehicle != nullptr &&
				!IsAttackHitObj(vehicle))
			{
				AddAttackHitObj(vehicle);
				// �U���͕��̃_���[�W��^����
				vehicle->TakeDamage(GetAttackPower(), this);
			}
		}
	}
	// �N���e�B�J���U���R���C�_�\
	else if (self == mpCriticalCol)
	{
		// �Փ˂������肪�G�Ȃ�
		if (other->Layer() == ELayer::eEnemy)
		{
			// �G�N���X���擾
			CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());

			if (enemy != nullptr &&
				!IsAttackHitObj(enemy))
			{
				AddAttackHitObj(enemy);
				// �������瑊��̕���
				CVector direction = enemy->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
				// �������^����m�b�N�o�b�N���x��2�{���������瑊��̕����ɐݒ�
				enemy->SetKnockbackReceived(direction * GetKnockbackDealt() * 2.0f);
				// �N���e�B�J���_���[�W��^����
				enemy->TakeCritical(GetAttackPower(), this, GetPower());
			}
		}
		// �Փ˂������肪������Ȃ�
		else if (other->Layer() == ELayer::eCollector)
		{
			// ������N���X���擾
			CCollector* collector = dynamic_cast<CCollector*>(other->Owner());

			if (collector != nullptr &&
				!IsAttackHitObj(collector))
			{
				AddAttackHitObj(collector);
				// �U���͕��̃_���[�W��^����
				collector->TakeDamage(GetAttackPower(), this);
			}
		}
		// �Փ˂������肪�ԗ��Ȃ�
		else if (other->Layer() == ELayer::eVehicle)
		{
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			if (vehicle != nullptr &&
				!IsAttackHitObj(vehicle))
			{
				AddAttackHitObj(vehicle);
				// �U���͕��̃_���[�W��^����
				vehicle->TakeCritical(GetAttackPower(), this);
			}
		}
	}
}

// ����������Ă��邩���擾
bool CTrashPlayer::GetStickCollector() const
{
	return mIsStickCollector;
}

// ����������Ă��邩��ݒ�
void CTrashPlayer::SetStickCollector(bool stickCollector)
{
	mIsStickCollector = stickCollector;
}

// ���Ă��������̃|�C���^���擾
CCollector* CTrashPlayer::GetStickCollectorPointer() const
{
	return mpStickCollector;
}

// ���Ă��������̃|�C���^��ݒ�
void CTrashPlayer::SetStickCollectorPointer(CCollector* collector)
{
	mpStickCollector = collector;
}

// �J���Ă��邩���擾
bool CTrashPlayer::GetOpen() const
{
	return mIsOpen;
}

// X���͈̔͊O�ɂ��邩�ǂ���
bool CTrashPlayer::AreaOutX()
{
	// �ԓ���X�͈̔͊O�ɂ���Ȃ�
	if (Position().X() >= ROAD_X_AREA ||
		Position().X() <= -ROAD_X_AREA)
	{
		return true;
	}
	// �����łȂ��Ȃ�͈͓�
	return false;
}

// Z���͈̔͊O�ɂ��邩�ǂ���
bool CTrashPlayer::AreaOutZ()
{
	// �ԓ���Z�͈̔͊O�ɂ���Ȃ�
	if (Position().Z() >= ROAD_Z_AREA ||
		Position().Z() <= -ROAD_Z_AREA)
	{
		return true;
	}
	// �����łȂ��Ȃ�͈͓�
	return false;
}

// �S�~�܂𗎂Ƃ�����
void CTrashPlayer::DropTrashBag(int power)
{
	// ���Ƃ��͂�0�ȉ��Ȃ珈�����Ȃ�
	if (power <= 0) return;

	// �S�~�܂���ł��������Ă����痎�Ƃ�
	if (GetTrashBag() > 0)
	{
		// �p���[�̍ŏI�I�Ȍ���
		int powerResult = power;
		// �S�~�܂̐����p���[��菭�Ȃ��ꍇ
		if (GetTrashBag() < power)
		{
			// �p���[�̍ŏI�I�Ȍ��ʂ��S�~�܂̐��ɐݒ�
			powerResult = GetTrashBag();
		}
		// �S�~�܂̐����ŏI�I�ȃp���[�����炷
		SetTrashBag(-powerResult);
		for (int i = 0; i < powerResult; i++)
		{
			CTrashBag* trashBag = new CTrashBag(false);
			trashBag->Position(Position() + TRASH_BAG_OFFSET_POS * (i + 1));
			trashBag->SetThrowSpeed(VectorZ() * GetKnockbackDealt(), GetKnockbackDealt());
		}
	}
	// �ʏ�̃S�~�܂���������Ă��Ȃ��ꍇ����
	// �S�[���h�S�~�܎����Ă���ꍇ�ɗ��Ƃ�
	else if (GetGoldTrashBag() > 0)
	{
		// �p���[�̍ŏI�I�Ȍ���
		int powerResult = power;
		// �S�~�܂̐����p���[��菭�Ȃ��ꍇ
		if (GetGoldTrashBag() < power)
		{
			// �p���[�̍ŏI�I�Ȍ��ʂ��S�~�܂̐��ɐݒ�
			powerResult = GetGoldTrashBag();
		}
		// �S�~�܂̐����ŏI�I�ȃp���[�����炷
		SetGoldTrashBag(-powerResult);
		for (int i = 0; i < powerResult; i++)
		{
			CTrashBag* trashBag = new CTrashBag(true);
			trashBag->Position(Position() + TRASH_BAG_OFFSET_POS * (i + 1));
			trashBag->SetThrowSpeed(VectorZ() * GetKnockbackDealt(), GetKnockbackDealt());
		}
	}
}

/*
�A�N�V�����̃L�[����
����������Ă��Ȃ��Ƃ��̂ݓ��͉\
*/
void CTrashPlayer::ActionInput()
{
	// ��������������Ă��邩��
	if (GetStickCollector())
	{
		// ������̃|�C���^
		CCollector* collector = GetStickCollectorPointer();
		// �|�C���^��null�łȂ����A
		// ����������d�u���p�̏ꍇ�L�[���͕s��
		if (collector != nullptr &&
			GetStickCollectorPointer()->GetPunisher())
		{
			return;
		}
	}

	// �X�y�[�X�ŃW�����v
	if (CInput::PushKey(VK_SPACE))
	{
		if (mState == EState::eOpenClose)
		{
			// �W�����v���x�̐ݒ�
			mMoveSpeedY = GetJumpSpeed();
			mIsGrounded = false;
			mIsJump = true;
			ChangeState(EState::eJump);
		}
		else
		{
			ChangeState(EState::eJumpStart);
		}
	}
	// ���N���b�N�ōU��
	if (CInput::PushKey(VK_LBUTTON))
	{
		// 1����100�܂ł�100�̐����痐�����擾
		int random = Math::Rand(1, 100);
		// �N���e�B�J���m���ȉ��̒l�Ȃ�N���e�B�J���U��
		if (random <= GetCriticalChance())
		{
			ChangeState(EState::eCriticalStart);
		}
		// ����ȊO�̎��͒ʏ�U��
		else
		{
			ChangeState(EState::eAttackStart);
		}
	}
	// ����������Ă��Ȃ��Ƃ��̂݃L�[���͂ł���
	if (!mIsStickCollector)
	{
		// �E�N���b�N�ŊW�̊J��
		if (CInput::PushKey(VK_RBUTTON))
		{
			ChangeState(EState::eOpenClose);
		}
	}
}

// �ҋ@���
void CTrashPlayer::UpdateIdle()
{
	if (mIsGrounded)
	{
		// �L�[���͉\
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

		// �W�����v�����U���J�n�I���A
		// �N���e�B�J���J�n�I���̏�ԂłȂ���΁A
		// �ړ��A�j���[�V�����ɐ؂�ւ�
		if (!mIsJump &&
			mState != EState::eAttackStart &&
			mState != EState::eAttackEnd&&
			mState!=EState::eCriticalStart&&
			mState != EState::eCriticalEnd)
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
	switch (mStateStep)
	{
	case 0:
		// �W�����v���ɍU�����ꂽ�Ƃ��̂���false
		mIsJump = false;
		mIsDamage = true;
		mMoveSpeed = CVector::zero;
		// ���Ă�����
		if (!mIsOpen)
		{
			// ������e�J�n�A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eDamage_Close_Start);
		}
		// �J���Ă�����
		else
		{
			// �J������e�J�n�A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eDamage_Open_Start);
		}
		mStateStep++;
		break;

	case 1:
		// �m�b�N�o�b�N���̔�яオ��̑��x��
		// �󂯂�m�b�N�o�b�N���x�̔����ɐݒ�
		mMoveSpeedY = GetKnockbackReceived().Length() * 0.5f;
		// �󂯂�m�b�N�o�b�N���x���ړ��ɐݒ�
		mMoveSpeed = GetKnockbackReceived();
		mIsGrounded = false;

		// ��e��Ԃ�
		ChangeState(EState::eDamage);
		break;
	}
}

// ��e�m�b�N�o�b�N
void CTrashPlayer::UpdateDamage()
{
	switch(mStateStep)
	{
	case 0:
		// �J���Ă���
		mIsOpen = true;
		// ��e�A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eDamage);
		mStateStep++;
		break;

	case 1:
		// �n�ʂɂ�����
		if (mIsGrounded)
		{
			// ��e�I����Ԃ�
			ChangeState(EState::eDamageEnd);
		}
		break;
	}
}

// ��e�I��
void CTrashPlayer::UpdateDamageEnd()
{
	switch (mStateStep)
	{
	case 0:
		// �󂯂�m�b�N�o�b�N�����Z�b�g
		SetKnockbackReceived(CVector::zero);
		// �ړ����[��
		mMoveSpeed = CVector::zero;
		// ��e�I���A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eDamage_End);
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V������60%�i�s������
		if (GetAnimationFrameRatio() >= 0.60f)
		{
			mIsDamage = false;
			mStateStep++;
		}
		break;

	case 2:
		// ���͉\
		ActionInput();
		// �ړ��\
		UpdateMove();
		// �A�j���[�V�������I��������ҋ@��
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// �W�����v�J�n
void CTrashPlayer::UpdateJumpStart()
{
	switch (mStateStep)
	{
	case 0:
		// �W�����v�J�n���͈ړ����[��
		mMoveSpeed = CVector::zero;
		// ���Ă�����
		if (!mIsOpen)
		{
			// ���Ă���W�����v�A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eJump_Close_Start);
		}
		// �J���Ă�����
		else
		{
			// �J���Ă���W�����v�A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eJump_Open_Start);
		}
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �W�����v���x�̐ݒ�
			mMoveSpeedY = GetJumpSpeed();
			mIsGrounded = false;
			mIsJump = true;
			// �W�����v����Ԃ�
			ChangeState(EState::eJump);
		}
		break;
	}
}

// �W�����v��
void CTrashPlayer::UpdateJump()
{
	switch (mStateStep)
	{
	case 0:
		// ���Ă�����
		if (!mIsOpen)
		{
			// ���Ă���W�����v���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eJump_Close);
		}
		// �J���Ă�����
		else
		{
			// �J���Ă���W�����v���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eJump_Open);
		}
		mStateStep++;
		break;

	case 1:
		// �W�����v���͈ړ����Ȃ���J�\
		// �E�N���b�N�ŊW�̊J��
		if (CInput::PushKey(VK_RBUTTON))
		{
			// �J��Ԃ�
			ChangeState(EState::eOpenClose);
		}
		// �n�ʂɕt������
		if (mIsGrounded)
		{
			// �W�����v�I����Ԃ�
			ChangeState(EState::eJumpEnd);
		}
		break;
	}
}

// �W�����v�I��
void CTrashPlayer::UpdateJumpEnd()
{
	switch (mStateStep)
	{
	case 0:
		mIsJump = false;
		// �ړ����[��
		mMoveSpeed = CVector::zero;
		// ���Ă�����
		if (!mIsOpen)
		{
			// ���Ă���W�����v�I���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eJump_Close_End);
		}
		// �J���Ă�����
		else
		{
			// �J���Ă���W�����v�I���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eJump_Open_End);
		}
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V������50%�ȏ�i�s�����烂�[�V�����L�����Z���\
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
			// �ҋ@��Ԃ�
			ChangeState(EState::eIdle);
		}
		break;
	}
}


// �U���J�n
void CTrashPlayer::UpdateAttackStart()
{
	switch (mStateStep)
	{
	case 0:
		// ���Ă���Ƃ�
		if (!mIsOpen)
		{
			// �J���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eOpen);
			// ���̃X�e�b�v��
			mStateStep++;
		}
		// �J���Ă���Ƃ�
		else
		{
			// �U���J�n�A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eAttack_Start);
			// �X�e�b�v2��
			mStateStep = 2;
		}
		break;

	case 1:
		// �J���A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			mIsOpen = true;
			// �U���J�n�A�j���[�V�������Đ�
			ChangeAnimation((int)EAnimType::eAttack_Start);
			mStateStep++;
		}
		break;

	case 2:
		// �U���J�n�A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �U������
			ChangeState(EState::eAttack);
			mMoveSpeed = CVector::zero;
		}
		break;
	}
}

// �U����
void CTrashPlayer::UpdateAttack()
{
	switch (mStateStep)
	{
	case 0:
		// �U���A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eAttack);
		AttackStart();
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �U���I����
			ChangeState(EState::eAttackEnd);
			AttackEnd();
		}
		break;
	}
}

// �U���I��
void CTrashPlayer::UpdateAttackEnd()
{
	switch (mStateStep)
	{
	case 0:
		mIsOpen = true;
		// �U���I���A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eAttack_End);
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V������52%�ȏ�i�s�����烂�[�V�����L�����Z���\
		if (GetAnimationFrameRatio() >= 0.52f)
		{
			// ���͉\
			ActionInput();
			// �ړ��\
			UpdateMove();
		}
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �ҋ@��Ԃ�
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// �N���e�B�J���J�n
void CTrashPlayer::UpdateCriticalStart()
{
	switch (mStateStep)
	{
	case 0:
		// ���Ă���Ƃ�
		if (!mIsOpen)
		{
			// �J���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eOpen);
			// ���̃X�e�b�v��
			mStateStep++;
		}
		// �J���Ă���Ƃ�
		else
		{
			// �N���e�B�J���J�n�A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eCritical_Start);
			// �X�e�b�v2��
			mStateStep = 2;
		}
		break;

	case 1:
		// �J���A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			mIsOpen = true;
			// �N���e�B�J���J�n�A�j���[�V�������Đ�
			ChangeAnimation((int)EAnimType::eCritical_Start);
			mStateStep++;
		}
		break;

	case 2:
		// �N���e�B�J���J�n�A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �N���e�B�J������
			ChangeState(EState::eCritical);
			mMoveSpeed = CVector::zero;
		}
		break;
	}
}

// �N���e�B�J����
void CTrashPlayer::UpdateCritical()
{
	switch (mStateStep)
	{
	case 0:
		// �N���e�B�J���A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eCritical);
		AttackStart();
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �N���e�B�J���I����
			ChangeState(EState::eCriticalEnd);
		}
		break;
	}
}

// �N���e�B�J���I��
void CTrashPlayer::UpdateCriticalEnd()
{
	switch (mStateStep)
	{
	case 0:
		// �N���e�B�J���I���A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eCritical_End);
		AttackEnd();
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V������52%�ȏ�i�s�����烂�[�V�����L�����Z���\
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
			// �ҋ@��Ԃ�
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// �W���J����
void CTrashPlayer::UpdateOpenClose()
{
	switch (mStateStep)
	{
	case 0:
		// �W�����v���Ă��Ȃ����
		if (!mIsJump)
		{
			// �ړ����[��
			mMoveSpeed = CVector::zero;
		}

		// ���Ă�����
		if (!mIsOpen)
		{
			// �J���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eOpen);
		}
		// �J���Ă�����
		else
		{
			// ����A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eClose);
		}
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V������50%�ȏ�i�s������
		if (GetAnimationFrameRatio() >= 0.50f)
		{
			// �J����Ԃ�ύX
			mIsOpen = !mIsOpen;
			mStateStep++;
		}
		break;

	case 2:
		// �W�����v���Ă��Ȃ����
		if (!mIsJump)
		{
			// ���͉\
			ActionInput();
			// �ړ��\
			UpdateMove();
		}
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �W�����v���Ă��Ȃ��Ȃ�
			if (!mIsJump)
			{
				// �ҋ@��Ԃ�
				ChangeState(EState::eIdle);
			}
			// �W�����v���Ă���Ȃ�W�����v�֖߂�
			else
			{
				// �W�����v��Ԃ�
				ChangeState(EState::eJump);
			}
		}
		break;
	}
}

// ���S�̍X�V����
void CTrashPlayer::UpdateDeath()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F���S�A�j���[�V�������Đ�
	case 0:
		ChangeAnimation((int)EAnimType::eDeath);
		mMoveSpeed = CVector::zero;
		mStateStep++;
		break;

		// �X�e�b�v1�F�A�j���[�V�������I�������玟�̃X�e�b�v��
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
		// �X�e�b�v1�FTODO�F�Q�[���I����ʂ�\������
	case 2:

		break;
	}
}

// ���S����
void CTrashPlayer::Death()
{
	// ���S��Ԃ�
	ChangeState(EState::eDeath);
}


// �U������
bool CTrashPlayer::IsAttacking() const
{
	// �U����
	if (mState == EState::eAttack) return true;
	// �N���e�B�J���U����
	if (mState == EState::eCritical) return true;

	// �U�����łȂ�
	return false;
}

// �U���J�n
void CTrashPlayer::AttackStart()
{
	// �x�[�X�N���X�̍U���J�n�������Ăяo��
	CXCharacter::AttackStart();

	// �U�����Ȃ�A�U���R���C�_�[���I��
	if (mState == EState::eAttack)
	{
		mpAttackCol->SetEnable(true);
	}
	// �N���e�B�J���U�����Ȃ�A�N���e�B�J���R���C�_�\���I��
	else if (mState == EState::eCritical)
	{
		mpCriticalCol->SetEnable(true);
	}
}

// �U���I��
void CTrashPlayer::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CXCharacter::AttackEnd();

	// �U���R���C�_�[���I�t
	mpAttackCol->SetEnable(false);
	mpCriticalCol->SetEnable(false);
}

// �_���[�W���󂯂�
void CTrashPlayer::TakeDamage(int damage, CObjectBase* causer, int dropNum)
{

	// �J���Ă���΃_���[�W���󂯂�
	if (mIsOpen)
	{
		// �_���[�W���󂯂�
		CCharaStatusBase::TakeDamage(damage, causer);
		// �S�~�܂𗎂Ƃ�
		DropTrashBag(dropNum);

		// ���S���Ă��Ȃ����
		if (!IsDeath())
		{
			// �U��������������̕����֌���
			CVector targetPos = causer->Position();
			CVector vec = targetPos - Position();
			vec.Y(0.0f);
			Rotation(CQuaternion::LookRotation(vec.Normalized()));

			// ��e�J�n��Ԃֈڍs
			ChangeState(EState::eDamageStart);
		}
	}
	// ���Ă���΃m�[�_���[�W�ŊW���J���̂�
	else
	{
		// �J��Ԃֈڍs
		ChangeState(EState::eOpenClose);
	}
}

// �N���e�B�J���_���[�W���󂯂�
void CTrashPlayer::TakeCritical(int damage, CObjectBase* causer, int dropNum)
{
	// �J���Ă����2�{�̃_���[�W���󂯂�
	if (mIsOpen)
	{
		// �U���͂�2�{�̃_���[�W
		int criticalDamage = damage * 2;
		// ���Ƃ�����2�{�h���b�v
		int criticalDropNum = dropNum * 2;
		// �_���[�W���󂯂�
		CCharaStatusBase::TakeDamage(criticalDamage, causer);
		// �S�~�܂𗎂Ƃ�
		DropTrashBag(criticalDropNum);
	}
	// ���Ă��Ă��ʏ�̃_���[�W���󂯂�
	else
	{
		// �_���[�W���󂯂�
		CCharaStatusBase::TakeDamage(damage, causer);
		// �S�~�܂𗎂Ƃ�
		DropTrashBag(dropNum);
	}

	// ���S���Ă��Ȃ����
	if (!IsDeath())
	{
		// �U��������������̕����֌���
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// ��e�J�n��Ԃֈڍs
		ChangeState(EState::eDamageStart);
	}
}

// ��Ԑ؂�ւ�
void CTrashPlayer::ChangeState(EState state)
{
	// ���ɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	// �U�����ɑ��̏�Ԃɕς��Ƃ�
	// �U���I���������Ă�
	if (IsAttacking())
	{
		AttackEnd();
	}

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
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
	case EState::eDeath:			return "���S";
	}
	return "";
}
#endif
