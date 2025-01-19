#include "CCollector.h"
#include "CDebugFieldOfView.h"
#include "CPlayerBase.h"
#include "CTrashPlayer.h"
#include "CFieldBase.h"
#include "Primitive.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "CVehicleManager.h"
#include "Maths.h"

// TODO�F��ŏ����e�X�g�p
#include "CInput.h"

// �Փˑ���̎ԗ��N���X���擾���邽�߂�
// �ԗ��̃N���X�̃C���N���[�h
#include "CCar.h"
#include "CGarbageTruck.h"

// �R���C�_�̃C���N���[�h
#include "CColliderCapsule.h"
#include "CColliderSphere.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Collector\\anim\\"
/*
 ������̃A�j���[�V�����f�[�^�̃e�[�u��
 (�L)���܂������Ă����Ԃ̃A�j���[�V����
 (��)���܂������Ă��Ȃ���Ԃ̃A�j���[�V����
 (�L��)�������̏�Ԍ��p�̃A�j���[�V����
*/
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{ "",								true,	 0.0f,	1.0f},	// T�|�[�Y
	{ ANIM_PATH"Idle.x",				true,	30.0f,	1.0f},	// �ҋ@					�i���j
	{ ANIM_PATH"Idle_Bag.x",			true,	30.0f,	1.0f},	// �ҋ@					�i�L�j
	{ ANIM_PATH"Move.x",				true,	80.0f,	1.0f},	// �ړ�					�i���j
	{ ANIM_PATH"Move_Bag.x",			true,	80.0f,	1.0f},	// �ړ�					�i�L�j
	{ ANIM_PATH"GetBag.x",				false,	10.0f,	1.0f},	// �S�~�܊l��			�i���j
	{ ANIM_PATH"GetBag_End.x",			false,	10.0f,	1.0f},	// �S�~�܊l���I��		�i�L�j
	{ ANIM_PATH"Death.x",				true,	11.0f,	1.0f},	// ���S					�i�L���j
	{ ANIM_PATH"Attack_Start.x",		false,	51.0f,	1.0f},	// �U���J�n				�i���j
	{ ANIM_PATH"Attack_True.x",			false,	20.0f,	1.0f},	// �U������				�i���j
	{ ANIM_PATH"Attack_False.x",		true,	11.0f,	1.0f},	// �U�����s				�i���j
	{ ANIM_PATH"Attack_False_Fall.x",	false,	21.0f,	1.0f},	// �U�����s������		�i���j
	{ ANIM_PATH"Attack_False_StandUp.x",false,	20.0f,	1.0f},	// �����オ��			�i���j
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

#define ROAD_X_AREA 90.0f	// �ԓ���X�͈̔�

// �U���������̃I�t�Z�b�g���W
#define ATTACK_SUCCESS_OFFSET_POS CVector(0.0f,0.0f,5.0f)

//	�R���X�g���N�^
CCollector::CCollector(bool punisher)
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
	, CCollectorStatus(punisher)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsBag(false)
	, mIsAttackSuccess(false)
	, mpAttackChara(nullptr)
{
	// �A�j���[�V�����ƃ��f���̏�����
	InitAnimationModel("Collector", &ANIM_DATA);

	// �n�`�A�v���C���[�A�G�A�U���A�ԗ��A�L�����̒T�m�p
	// �ƏՓ˔��������{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(BODY_WIDTH - BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		CVector(-BODY_WIDTH + BODY_RADIUS * 10, BODY_HEIGHT, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,ELayer::eCharaSearch,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eAttackCol, ELayer::eVehicle });

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);
}

// �f�X�g���N�^
CCollector::~CCollector()
{
}

// �X�V
void CCollector::Update()
{
	// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ�
	switch (mState)
	{
	case EState::eIdle:				UpdateIdle();			break;
	case EState::ePatrol:			UpdatePatrol();			break;
	case EState::eChase:			UpdateChase();			break;
	case EState::eLost:				UpdateLost();			break;
	case EState::eAttackStart:		UpdateAttackStart();	break;
	case EState::eAttack:			UpdateAttack();			break;
	case EState::eAttackEnd:		UpdateAttackEnd();		break;
	}

	// �U�����������Ă���Ԃ̓v���C���[�̑O�ɌŒ�
	if (mIsAttackSuccess)
	{
		CPlayerBase* player = CPlayerBase::Instance();
		Position(player->Position() + ATTACK_SUCCESS_OFFSET_POS);
	}

	// �L�����N�^�[�̍X�V
	CEnemyBase::Update();

#if _DEBUG
	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("CollectorState:%s\n", GetStateStr(mState).c_str());
#endif
}

// �Փˏ���
void CCollector::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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

			// �������U���J�n��Ԃ��U�������łɐ������Ă��Ȃ��ꍇ
			if (mState==EState::eAttackStart &&
				!mIsAttackSuccess)
			{
				// �v���C���[�N���X���擾
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

				if (player != nullptr &&
					!IsAttackHitObj(player))
				{
					AddAttackHitObj(player);
					mIsAttackSuccess = true; // �U��������
				}
			}
		}
	}
}

// �`��
void CCollector::Render()
{
	CEnemyBase::Render();

	// �����Ԃł���΁A
	if (mState == EState::ePatrol)
	{
		// ����|�C���g��S�ĕ`��
		int size = mPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireBox
			(
				mPatrolPoints[i]->GetPos() + CVector(0.0f, 1.0f, 0.0f),
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
				selfPos, playerPos,
				CColor::green,
				2.0f
			);
		}
	}
}

// �ҋ@���
void CCollector::UpdateIdle()
{
	mMoveSpeed = CVector::zero;
	// �v���C���[�̍��W���擾
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerPos = player->Position();
	// �v���C���[������͈͓����A���H���Ȃ�A
	// �ǐՏ�Ԃֈڍs
	if (IsFoundPlayer() &&
		playerPos.X() <= ROAD_X_AREA && playerPos.X() >= -ROAD_X_AREA)
	{
		ChangeState(EState::eChase);
		return;
	}

	// �ҋ@�A�j���[�V�������Đ�
	if (!mIsBag)
	{
		ChangeAnimation((int)EAnimType::eIdle);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eIdle_Bag);
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
void CCollector::UpdatePatrol()
{
	// �v���C���[�̍��W���擾
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerPos = player->Position();
	// �v���C���[������͈͓����A���H���Ȃ�A
	// �ǐՏ�Ԃֈڍs
	if (IsFoundPlayer() &&
		playerPos.X() <= ROAD_X_AREA && playerPos.X() >= -ROAD_X_AREA)
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
		if (!mIsBag)
		{
			ChangeAnimation((int)EAnimType::eMove);
		}
		else
		{
			ChangeAnimation((int)EAnimType::eMove_Bag);
		}
		if (mMoveRoute.size() == 1)
		{
			mNextMoveIndex = 0;
		}
		// �ŒZ�o�H�̎��̃m�[�h�܂ňړ�
		CNavNode* moveNode = mMoveRoute[mNextMoveIndex];

		if (MoveTo(moveNode->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			// �ړ����I���΁A���̃m�[�h�֐؂�ւ�
			mNextMoveIndex++;
			// �Ō�̃m�[�h�i�ړI�n�̃m�[�h�j�������ꍇ�́A���̃X�e�b�v�֐i�߂�
			if (mNextMoveIndex >= mMoveRoute.size())
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
		if (!mIsBag)
		{
			ChangeAnimation((int)EAnimType::eIdle);
		}
		else
		{
			ChangeAnimation((int)EAnimType::eIdle_Bag);
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
void CCollector::UpdateChase()
{
	// �v���C���[�̍��W�֌����Ĉړ�����
	CPlayerBase* player = CPlayerBase::Instance();
	CVector targetPos = player->Position();
	// �v���C���[�̍��W�����H�O�Ȃ�ǂ�������̂���߂�
	if (targetPos.X() >= ROAD_X_AREA ||
		targetPos.X() <= -ROAD_X_AREA)
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
		ChangeState(EState::eAttackStart);
		return;
	}

	// �ړ��A�j���[�V�������Đ�
	if (!mIsBag)
	{
		ChangeAnimation((int)EAnimType::eMove);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eMove_Bag);
	}

	// �o�H�T���Ǘ��N���X�����݂����
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// �o�H�T���p�̃m�[�h�̍��W���X�V
		mpNavNode->SetPos(Position());

		// ���g�̃m�[�h����v���C���[�̃m�[�h�܂ł̍ŒZ�o�H�����߂�
		CNavNode* playerNode = player->GetNavNode();
		if (navMgr->Navigate(mpNavNode, playerNode, mMoveRoute))
		{
			// ���g�̃m�[�h����v���C���[�̃m�[�h�܂Ōq�����Ă�����A
			// �ړ�����ʒu�����̃m�[�h�̈ʒu�ɐݒ�
			targetPos = mMoveRoute[1]->GetPos();
		}
	}
	// �ړ�����
	if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
	{

	}
}

// ����������
void CCollector::UpdateLost()
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
	if (!mIsBag)
	{
		ChangeAnimation((int)EAnimType::eMove);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eMove_Bag);
	}

	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0�F���������ʒu�܂ł̍ŒZ�o�H�����߂�
	case 0:
		// �o�H�T���p�̃m�[�h�̍��W���X�V
		mpNavNode->SetPos(Position());

		if (navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
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
		if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				// �ړ����I���΁A�ҋ@��Ԃֈڍs
				ChangeState(EState::eIdle);
				mpLostPlayerNode->SetEnable(false);
			}
		}
		break;
	}
}

// TODO�F�S�~���W�Ԃɖ߂鏈��
void CCollector::UpdateReturn()
{
}

// �U���J�n
void CCollector::UpdateAttackStart()
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
		// ������̈ړ���ݒ�
		mMoveSpeedY = GetJumpSpeed();
		// �U���J�n
		AttackStart();
		// �U���J�n�A�j���[�V�������Đ�
		ChangeAnimation((int)EAnimType::eAttack_Start);
		mStateStep++;
		break;

	case 1:
		// �U���J�n�A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �U������
			ChangeState(EState::eAttack);
		}
		break;
	}
}

// �U����
void CCollector::UpdateAttack()
{
	switch (mStateStep)
	{
	case 0:
		// �U�����������Ă���ꍇ
		if (mIsAttackSuccess)
		{
			// �U�������A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eAttack_True);
		}
		// ���s�Ȃ�
		else
		{
			// �U�����s�A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eAttack_False);
		}
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �U�����������Ă���ꍇ
			if (mIsAttackSuccess)
			{
				// �S�~�܊l���A�j���[�V�����Đ�
				ChangeAnimation((int)EAnimType::eGetBag);
			}
			// ���s�Ȃ�
			else
			{
				// �U�����s�������A�j���[�V�����Đ�
				ChangeAnimation((int)EAnimType::eAttack_False_Fall);
			}
			mStateStep++;
		}
		break;

	case 2:
		// �U�����������Ă���ꍇ
		if (mIsAttackSuccess)
		{
			// �A�j���[�V�������I��������
			if (IsAnimationFinished())
			{
				// TODO�F�S�~�܂�\��
				// �S�~�܂��擾
				mIsBag = true;
				// �U���I����Ԃ�
				ChangeState(EState::eAttackEnd);
			}
		}
		// ���s�Ȃ�
		else
		{
			// �n�ʂɕt������
			if (mIsGrounded)
			{
				// �U���I����Ԃ�
				ChangeState(EState::eAttackEnd);
			}
		}
	}
}

// �U���I��
void CCollector::UpdateAttackEnd()
{
	switch (mStateStep)
	{
	case 0:
		// �U���I��
		AttackEnd();
		// �U�����������Ă���ꍇ
		if (mIsAttackSuccess)
		{
			// �S�~�܊l���I���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eGetBag_End);
		}
		// ���s�Ȃ�
		else
		{
			// �U���I���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eAttack_False_StandUp);
		}
		mStateStep++;
		break;

	case 1:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �S�~���W�Ԃɖ߂��Ԃ�
			ChangeState(EState::eReturn);
		}
		break;
	}
}

// TODO�F���S����
void CCollector::Death()
{
}


// �U������
bool CCollector::IsAttacking() const
{
	// �U����
	if (mState == EState::eAttack) return true;

	// �U�����łȂ�
	return false;
}

// �U���J�n
void CCollector::AttackStart()
{
	// �x�[�X�N���X�̍U���J�n�������Ăяo��
	CXCharacter::AttackStart();
}

// �U���I��
void CCollector::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CXCharacter::AttackEnd();
}

// �_���[�W���󂯂�
void CCollector::TakeDamage(int damage, CObjectBase* causer)
{
	CCharaStatusBase::TakeDamage(damage, causer);
}

// ����ł��邩�ǂ���
bool CCollector::IsDead()
{
	return IsDeath();
}


// ��Ԑ؂�ւ�
void CCollector::ChangeState(EState state)
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
std::string CCollector::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return "�ҋ@";
	case EState::ePatrol:			return "����";
	case EState::eChase:			return "�ǐ�";
	case EState::eLost:				return "������";
	case EState::eReturn:			return "�S�~���W�Ԃɖ߂�";
	case EState::eAttackStart:		return "�U���J�n";
	case EState::eAttack:			return "�U����";
	case EState::eAttackEnd:		return "�U���I��";
	}
	return "";
}

// ��Ԃ̐F���擾
CColor CCollector::GetStateColor(EState state) const
{
	switch (state)
	{
	case EState::eIdle:				return CColor::white;
	case EState::ePatrol:			return CColor::green;
	case EState::eChase:			return CColor::red;
	case EState::eLost:				return CColor::yellow;
	case EState::eReturn:			return CColor::blue;
	case EState::eAttackStart:		return CColor::magenta;
	case EState::eAttack:			return CColor::magenta;
	case EState::eAttackEnd:		return CColor::magenta;
	}
	return CColor::white;
}
#endif
