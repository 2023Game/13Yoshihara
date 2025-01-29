#include "CTrashEnemy.h"
#include "CDebugFieldOfView.h"
#include "CPlayerBase.h"
#include "CTrashPlayer.h"
#include "CFieldBase.h"
#include "Primitive.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "CVehicleManager.h"
#include "Maths.h"
#include "CGaugeUI3D.h"
#include "CTrashBag.h"

// TODO�F��ŏ����e�X�g�p
#include "CInput.h"

// �Փˑ���̎ԗ��N���X���擾���邽�߂�
// �ԗ��̃N���X�̃C���N���[�h
#include "CCar.h"
#include "CGarbageTruck.h"

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
	{ "",								true,	 0.0f,	1.0f},	// T�|�[�Y
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

#define FOV_ANGLE 45.0f		// ����͈͂̊p�x
#define FOV_LENGTH 100.0f	// ����͈͂̋���
#define EYE_HEIGHT 5.0f		// ���_�̍���

#define ROTATE_SPEED 9.0f	// ��]���x
#define ATTACK_RANGE 18.0f	// �U�����鋗��

#define PATROL_INTERVAL	3.0f	// ���̏���|�C���g�Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST 10.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME 5.0f			// �ҋ@��Ԃ̎���

#define PATROL_POS0 CVector( 40.0f,0.0f,  0.0f)
#define PATROL_POS1 CVector( 40.0f,0.0f,100.0f)
#define PATROL_POS2 CVector(  0.0f,0.0f,100.0f)
#define PATROL_POS3 CVector(-40.0f,0.0f,100.0f)
#define PATROL_POS4 CVector(-40.0f,0.0f,  0.0f)
#define PATROL_POS5 CVector(-40.0f,0.0f,-100.0f)
#define PATROL_POS6 CVector(  0.0f,0.0f,-100.0f)
#define PATROL_POS7 CVector( 40.0f,0.0f,-100.0f)

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

// ���񂾂Ƃ��̏�����܂ł̎���
#define DEATH_WAIT_TIME 2.0f

// Hp�Q�[�W�̃I�t�Z�b�g���W
#define GAUGE_OFFSET_POS CVector(0.0f,15.0f,0.0f)
// Hp�Q�[�W�̉摜�̃p�X
#define HP_GAUGE_PATH "UI\\trashbox_enemy_hp_gauge.png"

// �S�~�܂𗎂Ƃ��I�t�Z�b�g���W
#define TRASH_BAG_OFFSET_POS CVector(0.0f,5.0f,0.0f)

// �R���X�g���N�^
CTrashEnemy::CTrashEnemy(bool punisher, float scale)
	: CEnemyBase
	(
		FOV_ANGLE,
		FOV_LENGTH,
		{
			PATROL_POS0,
			PATROL_POS1,
			PATROL_POS2,
			PATROL_POS3,
			PATROL_POS4,
			PATROL_POS5,
			PATROL_POS6,
			PATROL_POS7
		},
		EYE_HEIGHT
	)
	, CTrashEnemyStatus(punisher)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsOpen(false)
	, mIsJump(false)
{
	// Hp�Q�[�W��ݒ�
	mpHpGauge = new CGaugeUI3D(this, HP_GAUGE_PATH);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());
	Scale(scale, scale, scale);
	// �A�j���[�V�����ƃ��f���̏�����
	InitAnimationModel("TrashEnemy", &ANIM_DATA);


	// �{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(BODY_WIDTH - BODY_RADIUS / scale, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS / scale, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	// �n�`�A�v���C���[�A�G�A������A�U���A�ԗ��A�L�����̒T�m�p�A�S�~��
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle, ETag::eTrashBag });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,ELayer::eCharaSearch,
		ELayer::ePlayer, ELayer::eEnemy,ELayer::eCollector, ELayer::eAttackCol, ELayer::eVehicle, ELayer::eTrashBag});

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

	// �v���C���[�ƏՓ˔��肷��悤�ɐݒ�
	mpAttackCol->SetCollisionTags({ ETag::ePlayer});
	mpAttackCol->SetCollisionLayers({ ELayer::ePlayer});
	mpCriticalCol->SetCollisionTags({ ETag::ePlayer});
	mpCriticalCol->SetCollisionLayers({ ELayer::ePlayer});

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
CTrashEnemy::~CTrashEnemy()
{
	SAFE_DELETE(mpCriticalCol);
}

// �X�V
void CTrashEnemy::Update()
{
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
	case EState::eDeath:			UpdateDeath();			break;
	}

	// �L�����N�^�[�̍X�V
	CEnemyBase::Update();

	// HP�Q�[�W���X�V
	mpHpGauge->Position(Position() + GAUGE_OFFSET_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());

#if _DEBUG
	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("EnemyState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("EnemyIsOpen:%s\n", mIsOpen ? "true" : "false");
	CDebugPrint::Print("EnemyIsJump:%s\n", mIsJump ? "true" : "false");
	CDebugPrint::Print("EnemyHp:%d\n", GetHp());
	CDebugPrint::Print("EnemyTrashBagNum:%d\n", GetTrashBag());
	CDebugPrint::Print("EnemyGoldBagNum:%d\n", GetGoldTrashBag());
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
		// �Փ˂������肪������Ȃ�
		else if (other->Layer() == ELayer::eCollector)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
	// �U���R���C�_�[
	else if (self == mpAttackCol)
	{
		// �Փ˂������肪�v���C���[�Ȃ�
		if (other->Layer() == ELayer::ePlayer)
		{
			// �v���C���[�N���X���擾
			CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

			if (player != nullptr &&
				!IsAttackHitObj(player))
			{
				AddAttackHitObj(player);
				// �������瑊��̕���
				CVector direction = player->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
				// �������^����m�b�N�o�b�N���x���������瑊��̕����ɐݒ�
				player->SetKnockbackReceived(direction * GetKnockbackDealt());
				// �U���͕��̃_���[�W��^����
				player->TakeDamage(GetAttackPower(), this, GetPower());
			}
		}
	}
	// �N���e�B�J���U���R���C�_�\
	else if (self == mpCriticalCol)
	{
		// �Փ˂������肪�v���C���[�Ȃ�
		if (other->Layer() == ELayer::ePlayer)
		{
			// �v���C���[�N���X���擾
			CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

			if (player != nullptr &&
				!IsAttackHitObj(player))
			{
				AddAttackHitObj(player);
				// �������瑊��̕���
				CVector direction = player->Position() - Position();
				direction = direction.Normalized();
				direction.Y(0.0f);
				// ���肪�󂯂�m�b�N�o�b�N���x�ɁA
				// �������^����m�b�N�o�b�N���x��2�{���������瑊��̕����ɐݒ�
				player->SetKnockbackReceived(direction * GetKnockbackDealt() * 2.0f);
				// �U���͕��̃_���[�W��^����
				player->TakeCritical(GetAttackPower(), this, GetPower());
			}
		}
	}
}

// �`��
void CTrashEnemy::Render()
{
	CEnemyBase::Render();

	// �����Ԃł���΁A
	if (mState == EState::ePatrol)
	{
		// ����|�C���g��S�ĕ`��
		int size = mpPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireBox
			(
				mpPatrolPoints[i]->GetPos() + CVector(0.0f, 1.0f, 0.0f),
				CVector::one,
				c
			);
		}
	}
	// ����������Ԃł���΁A
	else if (mState == EState::eLost)
	{
		// �v���C���[�����������ʒu�Ƀf�o�b�O�\��
		Primitive::DrawWireBox
		(
			mpLostPlayerNode->GetPos() + CVector(0.0f, 1.0f, 0.0f),
			CVector::one,
			CColor::blue
		);
	}

	CPlayerBase* player = CPlayerBase::Instance();
	CFieldBase* field = CFieldBase::Instance();
	CVehicleManager* vehicleMgr = CVehicleManager::Instance();
	if (player != nullptr && field != nullptr)
	{
		CVector offsetPos = CVector(0.0f, mEyeHeight, 0.0f);
		CVector playerPos = player->Position() + offsetPos;
		CVector selfPos = Position() + offsetPos;

		// �v���C���[�Ƃ̊ԂɎՕ��������݂���ꍇ
		CHitInfo hit;
		bool isHit = false;
		// �t�B�[���h�Ƃ̃��C����
		if (field->CollisionRay(selfPos, playerPos, &hit))
		{
			isHit = true;
		}
		// �ԗ��Ƃ̃��C����
		if (vehicleMgr->CollisionRay(selfPos, playerPos, &hit, isHit))
		{
			isHit = true;
		}
		if (isHit)
		{
			// �Փ˂����ʒu�܂ŐԐ���`��
			Primitive::DrawLine
			(
				selfPos, hit.cross,
				CColor::red,
				2.0f
			);
		}
		// �Օ��������݂��Ȃ������ꍇ
		else
		{
			// �v���C���[�̈ʒu�܂ŗΐ���`��
			Primitive::DrawLine
			(
				selfPos,playerPos,
				CColor::green,
				2.0f
			);
		}
	}
}

// �J���Ă��邩���擾
bool CTrashEnemy::GetOpen() const
{
	return mIsOpen;
}

// �S�~�܂𗎂Ƃ�����
void CTrashEnemy::DropTrashBag(int power)
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

// �ҋ@���
void CTrashEnemy::UpdateIdle()
{
	mMoveSpeed = CVector::zero;
	// �v���C���[���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// �v���C���[������͈͓����A���H���Ȃ�A
	// �ǐՏ�Ԃֈڍs
	if (IsFoundPlayer()&&
		!player->AreaOutX())
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
		mStateStep = 0;
	}
}

// ���񏈗�
void CTrashEnemy::UpdatePatrol()
{
	// �v���C���[���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// �v���C���[������͈͓����A���H���Ȃ�A
	// �ǐՏ�Ԃֈڍs
	if (IsFoundPlayer() &&
		!player->AreaOutX())
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
		ChangePatrolPoint(PATROL_NEAR_DIST);
		mStateStep++;
		break;

	// �X�e�b�v1�F����|�C���g�܂ňړ�
	case 1:
	{
		// �ړ��A�j���[�V�������Đ�
		if (!mIsOpen)
		{
			ChangeAnimation((int)EAnimType::eMove_Close);
		}
		else
		{
			ChangeAnimation((int)EAnimType::eMove_Open);
		}

		// �ŒZ�o�H�̎��̃m�[�h�܂ňړ�
		CNavNode* moveNode = mpMoveRoute[mNextMoveIndex];

		if (MoveTo(moveNode->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			// �ړ����I���΁A���̃m�[�h�֐؂�ւ�
			mNextMoveIndex++;
			// �Ō�̃m�[�h�i�ړI�n�̃m�[�h�j�������ꍇ�́A���̃X�e�b�v�֐i�߂�
			if (mNextMoveIndex >= mpMoveRoute.size())
			{
				mStateStep++;
			}
		}
		break;
	}
	// �X�e�b�v2�F�ړ���̑ҋ@
	case 2:
		mMoveSpeed = CVector::zero;
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
			ChangePatrolPoint(PATROL_NEAR_DIST);
			mStateStep = 1;
			mElapsedTime = 0.0f;
		}
	}
}

// �ǐՏ���
void CTrashEnemy::UpdateChase()
{
	// �v���C���[�̍��W�֌����Ĉړ�����
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	CVector targetPos = player->Position();
	// �v���C���[�̍��W�����H�O�Ȃ�ǂ�������̂���߂�
	if (player->AreaOutX())
	{
		ChangeState(EState::eIdle);
		return;
	}
	// �v���C���[�������Ȃ��Ȃ�����A����������Ԃֈڍs
	if (!IsLookPlayer())
	{
		// ���������ʒu�Ƀm�[�h��z�u
		mpLostPlayerNode->SetPos(targetPos);
		mpLostPlayerNode->SetEnable(true);
		ChangeState(EState::eLost);
		mStateStep = 0;
		return;
	}
	// �v���C���[�ɍU���ł���Ȃ�΁A�U����Ԃֈڍs
	if (CanAttackPlayer(ATTACK_RANGE))
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

	// �o�H�T���Ǘ��N���X�����݂����
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// �o�H�T���p�̃m�[�h�̍��W���X�V
		mpNavNode->SetPos(Position());

		// ���g�̃m�[�h����v���C���[�̃m�[�h�܂ł̍ŒZ�o�H�����߂�
		CNavNode* playerNode = player->GetNavNode();
		if (navMgr->Navigate(mpNavNode, playerNode, mpMoveRoute))
		{
			// ���g�̃m�[�h����v���C���[�̃m�[�h�܂Ōq�����Ă�����A
			// �ړ�����ʒu�����̃m�[�h�̈ʒu�ɐݒ�
			targetPos = mpMoveRoute[1]->GetPos();
		}
	}
	// �ړ�����
	if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
	{

	}
}

// ����������
void CTrashEnemy::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		ChangeState(EState::eIdle);
		return;
	}
	// �v���C���[����������A�ǐՏ�Ԃֈڍs
	if (IsLookPlayer())
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

	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
	// �X�e�b�v0�F���������ʒu�܂ł̍ŒZ�o�H�����߂�
	case 0:
		// �o�H�T���p�̃m�[�h�̍��W���X�V
		mpNavNode->SetPos(Position());

		if (navMgr->Navigate(mpNavNode, mpLostPlayerNode, mpMoveRoute))
		{
			// ���������ʒu�܂Ōo�H���q�����Ă�����A���̃X�e�b�v��
			mNextMoveIndex = 1;
			mStateStep++;
		}
		else
		{
			// �o�H���q�����Ă��Ȃ���΁A�ҋ@��Ԃ֖߂�
			ChangeState(EState::eIdle);
			mpLostPlayerNode->SetEnable(false);
		}
		break;
	case 1:
		// �v���C���[�����������ʒu�܂ňړ�
		if (MoveTo(mpMoveRoute[mNextMoveIndex]->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mpMoveRoute.size())
			{
				// �ړ����I���΁A�ҋ@��Ԃֈڍs
				ChangeState(EState::eIdle);
				mpLostPlayerNode->SetEnable(false);
			}
		}
		break;
	}
}

// ��e�J�n
void CTrashEnemy::UpdateDamageStart()
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
void CTrashEnemy::UpdateDamage()
{
	switch (mStateStep)
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
void CTrashEnemy::UpdateDamageEnd()
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
		// �A�j���[�V�������I��������ҋ@��
		if (IsAnimationFinished())
		{
			// �U�����Ă������肪�v���C���[�Ȃ�ǐՏ�Ԃ�
			if (mpDamageCauser = CPlayerBase::Instance())
			{
				ChangeState(EState::eChase);
			}
			// ����ȊO�Ȃ�ҋ@��Ԃ�
			else
			{
				ChangeState(EState::eIdle);
			}
			// �U�����Ă�������̋L�������Z�b�g����
			mpDamageCauser = nullptr;
		}
		break;
	}
}

// �W�����v�J�n
void CTrashEnemy::UpdateJumpStart()
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
void CTrashEnemy::UpdateJump()
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
void CTrashEnemy::UpdateJumpEnd()
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
void CTrashEnemy::UpdateAttackStart()
{
	// �v���C���[�̍��W
	CPlayerBase* player = CPlayerBase::Instance();
	CVector targetPos = player->Position();
	// �v���C���[�̕����ֈړ�
	if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
	{

	}
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
void CTrashEnemy::UpdateAttack()
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
void CTrashEnemy::UpdateAttackEnd()
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
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �ǐՏ�Ԃ�
			ChangeState(EState::eChase);
		}
		break;
	}
}

// �N���e�B�J���U���J�n
void CTrashEnemy::UpdateCriticalStart()
{
	// �v���C���[�̍��W
	CPlayerBase* player = CPlayerBase::Instance();
	CVector targetPos = player->Position();
	// �v���C���[�̕����ֈړ�
	if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
	{

	}

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

// �N���e�B�J���U����
void CTrashEnemy::UpdateCritical()
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

// �N���e�B�J���U���I��
void CTrashEnemy::UpdateCriticalEnd()
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
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �ǐՏ�Ԃ�
			ChangeState(EState::eChase);
		}
		break;
	}
}

// �W���J����
void CTrashEnemy::UpdateOpenClose()
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
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �W�����v���Ă��Ȃ�
			if (!mIsJump)
			{
				// �v���C���[�ɍU������Ă�����
				if (mpDamageCauser == CPlayerBase::Instance())
				{
					// �ǐՏ�Ԃ�
					ChangeState(EState::eChase);
					// �U�����Ă�������̋L�������Z�b�g
					mpDamageCauser = nullptr;
				}
				// ����ȊO�Ȃ�
				else
				{
					// �ҋ@��Ԃ�
					ChangeState(EState::eIdle);
				}
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
void CTrashEnemy::UpdateDeath()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F���S�A�j���[�V�������Đ�
	case 0:
		// �����蔻����I�t
		SetEnableCol(false);
		// �d�͂��I�t
		mIsGravity = false;
		ChangeAnimation((int)EAnimType::eDeath);
		mStateStep++;
		break;

		// �X�e�b�v1�F�A�j���[�V�������I�������玟�̃X�e�b�v��
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;

		// �X�e�b�v2�F�S�ẴS�~�܂𗎂Ƃ�
	case 2:
		// �S�~�܂�1�����Ƃ�
		DropTrashBag(1);
		// �ʏ�ƃS�[���h�̃S�~�܂������̐���0�ȉ��Ȃ�
		if (GetTrashBag() <= 0 &&
			GetGoldTrashBag() <= 0)
		{
			// ���̃X�e�b�v��
			mStateStep++;
		}
		break;
		// �X�e�b�v3�F������܂ł̎��ԂɂȂ�܂ŃJ�E���g
	case 3:
		mElapsedTime += Times::DeltaTime();
		if (mElapsedTime >= DEATH_WAIT_TIME)
		{
			mStateStep++;
		}
		break;
		// �X�e�b�v4�F���g�Ǝ���͈͂�HP�Q�[�W�͖����ɂȂ�
	case 4:
		SetEnable(false);
		SetShow(false);
		mpDebugFov->SetEnable(false);
		mpDebugFov->SetShow(false);
		mpHpGauge->SetEnable(false);
		mpHpGauge->SetShow(false);
		break;
	}
}

// ���S����
void CTrashEnemy::Death()
{
	// ���S��Ԃ�
	ChangeState(EState::eDeath);
}


// �U������
bool CTrashEnemy::IsAttacking() const
{
	// �U����
	if (mState == EState::eAttack) return true;
	// �N���e�B�J���U����
	if (mState == EState::eCritical) return true;

	// �U�����łȂ�
	return false;
}

// �U���J�n
void CTrashEnemy::AttackStart()
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
void CTrashEnemy::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CXCharacter::AttackEnd();

	// �U���R���C�_�[���I�t
	mpAttackCol->SetEnable(false);
	mpCriticalCol->SetEnable(false);
}

// �_���[�W���󂯂�
void CTrashEnemy::TakeDamage(int damage, CObjectBase* causer, int dropNum)
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
	// ���Ă���΃m�[�_���[�W�ŊW���J��
	else
	{
		// �J��Ԃֈڍs
		ChangeState(EState::eOpenClose);
	}
	mpDamageCauser = causer;
}

// �N���e�B�J���_���[�W���󂯂�
void CTrashEnemy::TakeCritical(int damage, CObjectBase* causer, int dropNum)
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
void CTrashEnemy::ChangeState(EState state)
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
	case EState::eDeath:			return "���S";
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
	case EState::eDeath:			return CColor::white;
	}
	return CColor::white;
}

#endif