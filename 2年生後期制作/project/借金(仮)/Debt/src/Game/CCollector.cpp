#include "CCollector.h"
#include "CDebugFieldOfView.h"
#include "CTrashPlayer.h"
#include "CPlayerBase.h"
#include "CFieldBase.h"
#include "Primitive.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "CVehicleManager.h"
#include "Maths.h"
#include "CTrashPlayer.h"
#include "CTrashBag.h"

// �Փˑ���̎ԗ��N���X���擾���邽�߂�
// �ԗ��̃N���X�̃C���N���[�h
#include "CCar.h"
#include "CGarbageTruck.h"

// �R���C�_�̃C���N���[�h
#include "CColliderCapsule.h"

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
	{ "",								 true,	  0.0f,	1.0f},	// T�|�[�Y
	{ ANIM_PATH"Idle.x",				 true,	 80.0f,	1.0f},	// �ҋ@					�i���j
	{ ANIM_PATH"Idle_Bag.x",			 true,	 80.0f,	1.0f},	// �ҋ@					�i�L�j
	{ ANIM_PATH"Move.x",				 true,	 80.0f,	1.0f},	// �ړ�					�i���j
	{ ANIM_PATH"Move_Bag.x",			 true,	 80.0f,	1.0f},	// �ړ�					�i�L�j
	{ ANIM_PATH"GetBag.x",				false,	240.0f,	1.0f},	// �S�~�܊l��			�i���j
	{ ANIM_PATH"GetBag_End.x",			false,	 60.0f,	1.0f},	// �S�~�܊l���I��		�i�L�j
	{ ANIM_PATH"Death.x",				false,	  5.0f,	1.0f},	// ���S					�i�L���j
	{ ANIM_PATH"Attack_Start.x",		false,	 50.0f,	1.0f},	// �U���J�n				�i���j
	{ ANIM_PATH"Attack_True.x",			false,	 20.0f,	1.0f},	// �U������				�i���j
	{ ANIM_PATH"Attack_False.x",		false,	 20.0f,	1.0f},	// �U�����s				�i���j
	{ ANIM_PATH"Attack_False_Fall.x",	 true,	 20.0f,	1.0f},	// �U�����s������		�i���j
	{ ANIM_PATH"Attack_False_StandUp.x",false,	 70.0f,	1.0f},	// �����オ��			�i���j
};

#define BODY_RADIUS 2.5f	// �{�̂̃R���C�_�\�̔��a
#define BODY_HEIGHT 50.0f	// �{�̂̃R���C�_�\�̍���

#define FOV_ANGLE 45.0f		// ����͈͂̊p�x
#define FOV_LENGTH 100.0f	// ����͈͂̋���
#define EYE_HEIGHT 5.0f		// ���_�̍���

#define ROTATE_SPEED 9.0f	// ��]���x
#define ATTACK_RANGE 18.0f	// �U�����鋗��

#define PATROL_INTERVAL	0.5f	// ���̏���|�C���g�Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST 10.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME 0.5f			// �ҋ@��Ԃ̎���

#define ROAD_X_AREA 90.0f	// �ԓ���X�͈̔�

// �U���������̃I�t�Z�b�g���W
#define ATTACK_SUCCESS_OFFSET_POSY 3.0f
// Z���W�̓v���C���[�̑O�����̃x�N�g�����|���ċ��߂�̂�float�^
#define ATTACK_SUCCESS_OFFSET_POSZ 4.0f

#define SCALE 0.2f	// �X�P�[��

// ���񂾂Ƃ��̏�����܂ł̎���
#define DEATH_WAIT_TIME 2.0f

// �S�~�܂̎����Ă��鎞�̃I�t�Z�b�g���W
#define HAVE_TRASH_BAG_OFFSET_POS		CVector(-0.5f,0.5f,0.5f)
// �S�~�܂̎����Ă���Ƃ��̉�]
#define HAVE_TRASH_BAG_OFFSET_ROTATION	CVector(0.0f,0.0f,90.0f)
// ���Ƃ����̃S�~�܂̃I�t�Z�b�g���W
#define TRASH_BAG_OFFSET_POS			CVector(0.0f, 5.0f, -2.0f)

// �S�~�܂̃X�P�[��
#define TRASH_BAG_SCALE CVector(0.3f,0.3f,0.3f)

//	�R���X�g���N�^
CCollector::CCollector(bool punisher, CObjectBase* owner,
	std::vector<CNavNode*> patrolPoints)
	: CEnemyBase
	(
		FOV_ANGLE,
		FOV_LENGTH,
		{},
		EYE_HEIGHT
	)
	, CCollectorStatus(punisher)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsBag(false)
	, mIsAttackSuccess(false)
	, mIsAttacking(false)
	, mpOwner(owner)
{
	// ����|�C���g��ݒ�
	mpPatrolPoints = patrolPoints;
	// ��{�d�͂�5����1�̏d��
	mGravity = mGravity * 0.2f;
	// �傫���̒���
	Scale(SCALE, SCALE, SCALE);
	// ���d�u���p�̏ꍇ
	if (punisher)
	{
		// �A�j���[�V�����ƃ��f���̏�����
		InitAnimationModel("PunisherCollector", &ANIM_DATA);
	}
	// �ʏ�̏ꍇ
	else
	{
		// �A�j���[�V�����ƃ��f���̏�����
		InitAnimationModel("Collector", &ANIM_DATA);
	}


	// �{�̂̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eCollector,
		CVector(0.0f, BODY_RADIUS / SCALE, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS / SCALE, 0.0f),
		BODY_RADIUS
	);
	// �n�`�A�v���C���[�A�G�A������A�U���A�ԗ��A�L�����̒T�m�p
	// �ƏՓ˔��������
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eEnemy, ETag::eVehicle });
	mpBodyCol->SetCollisionLayers({ ELayer::eGround, ELayer::eWall, ELayer::eObject,ELayer::eCharaSearch,
		ELayer::ePlayer, ELayer::eEnemy, ELayer::eCollector, ELayer::eAttackCol, ELayer::eVehicle });

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

	// ���p�̃S�~�܂𐶐�
	mpTrashBag = new CTrashBag
	(
		false
	);
	// �E��̃t���[�����擾���A
	// �S�~�܂ɉE��̍s����A�^�b�`
	CModelXFrame* frame = mpModel->FinedFrame("FoxRig_hand_R");
	mpTrashBag->SetAttachMtx(&frame->CombinedMatrix());
	// ��]�A���W�A�X�P�[���̐ݒ�
	mpTrashBag->Rotation(HAVE_TRASH_BAG_OFFSET_ROTATION);
	mpTrashBag->Position(HAVE_TRASH_BAG_OFFSET_POS);
	mpTrashBag->Scale(TRASH_BAG_SCALE);
	// �ŏ��͔�\��
	mpTrashBag->SetEnable(false);
	mpTrashBag->SetShow(false);
	// �Փ˔���͂��Ȃ�
	mpTrashBag->SetEnableCol(false);
	// �d�͂��|���Ȃ�
	mpTrashBag->SetGravity(false);
}

// �f�X�g���N�^
CCollector::~CCollector()
{
	if (mpOwner != nullptr)
	{
		mpOwner = nullptr;
	}
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
	case EState::eReturn:			UpdateReturn();			break;
	case EState::eAttackStart:		UpdateAttackStart();	break;
	case EState::eAttackTrue:		UpdateAttackTrue();		break;
	case EState::eAttackFalse:		UpdateAttackFalse();	break;
	case EState::eAttackEnd:		UpdateAttackEnd();		break;
	case EState::eDeath:			UpdateDeath();			break;
	}

	// �L�����N�^�[�̍X�V
	CEnemyBase::Update();

	// �S�~�܂������Ă���ꍇ
	if (mIsBag)
	{
		mpTrashBag->UpdateMtx();
	}

	// �������R���C�_�\�I�t�̎��ɏ������蔲�����Ƃ��̕ی�
	if (Position().Y() < 0.0f)
	{
		Position(Position().X(), 0.0f, Position().Z());
	}

#if _DEBUG
	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("CollectorState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("CollectorAttackSuccess:%s\n", mIsAttackSuccess ? "true" : "false");
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

			// ����̎�����Ǝ����̎����傪��v���Ă��邩�A
			// �������P�ޏ�ԂȂ�
			if (other->Owner() == mpOwner&&
				mState == EState::eReturn)
			{
				// �S�~���W�Ԃ̃N���X���擾
				CGarbageTruck* owner = dynamic_cast<CGarbageTruck*>(mpOwner);
				// �e��null�łȂ��ꍇ
				if (owner != nullptr)
				{
					// �����̎����Ă���S�~�܂̐���e�ɓn��
					owner->SetTrashBag(GetTrashBag());
					owner->SetGoldTrashBag(GetTrashBag());
				}
				// �����ɂ���
				SetOnOff(false);
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

			// �������U�������U�������łɐ������Ă��Ȃ��ꍇ
			if (IsAttacking() &&
				!mIsAttackSuccess)
			{
				// �v���C���[�N���X���擾
				CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(other->Owner());

				// �v���C���[�����݂��Ă܂��U�����������Ă��Ȃ�����
				// ����������ɂ��Ă��Ȃ��Ȃ�
				if (player != nullptr &&
					!IsAttackHitObj(player)&&
					!player->GetStickCollector())
				{
					AddAttackHitObj(player);
					mIsAttackSuccess = true;		// �U��������
					player->SetStickCollector(true);// ����������Ă���
					// ���̉���������Ă��������ɐݒ�
					player->SetStickCollectorPointer(this);
				}
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
}

// �`��
void CCollector::Render()
{
	CEnemyBase::Render();
#if _DEBUG
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
				selfPos, playerPos,
				CColor::green,
				2.0f
			);
		}
	}
#endif
}

// ������̗L��������؂�ւ���
void CCollector::SetOnOff(bool setOnOff)
{
	// �����ɂ���Ƃ�
	if (!setOnOff)
	{
		// �S�~���W�Ԃ̃N���X���擾
		CGarbageTruck* owner = dynamic_cast<CGarbageTruck*>(mpOwner);
		// �e��null�łȂ��ꍇ
		if (owner != nullptr)
		{
			// ������̐l�������炷
			owner->DecreaseCollectors();
		}
		// �S�~�܂������Ă��邩�����Z�b�g
		SetHaveBag(false);
		// �����Ă��鐔�����Z�b�g
		SetTrashBag(-GetTrashBag());
		SetGoldTrashBag(-GetGoldTrashBag());
#if _DEBUG
		if (mpDebugFov != nullptr)
		{
			mpDebugFov->SetEnable(false);
			mpDebugFov->SetShow(false);
		}
#endif
	}
	// �L���ɂ���Ƃ�
	else
	{
		// Hp�����Z�b�g����
		SetHp();
		// �S�~�܂������Ă��邩�����Z�b�g
		SetHaveBag(false);
		ChangeState(EState::eIdle);
		ChangeAnimation((int)EState::eIdle);
		SetEnableCol(true);
		mIsGravity = true;
#if _DEBUG
		if (mpDebugFov != nullptr)
		{
			mpDebugFov->SetEnable(true);
			mpDebugFov->SetShow(true);
		}
#endif
	}
	// �L��������ݒ肷��
	SetEnable(setOnOff);
	SetShow(setOnOff);
}

// �P����Ԃɕς���
void CCollector::ChangeStateReturn()
{
	// ���S��Ԃ���Ȃ��Ȃ�
	if (mState != EState::eDeath)
	{
		ChangeState(EState::eReturn);
	}
}

// ������̎������ݒ�
void CCollector::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ������̎�������擾
CObjectBase* CCollector::GetOwner() const
{
	return mpOwner;
}

// �ǐՏ�Ԃւ̈ڍs�̏������`�F�b�N
bool CCollector::ChangeChase()
{
	// �v���C���[���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// �v���C���[������͈͓����A��������������Ă��Ȃ����A
	// ���H���Ȃ�A
	// �ǐՏ�Ԃֈڍs
	if (IsFoundPlayer() &&
		!player->GetStickCollector() &&
		!player->AreaOutX())
	{
		ChangeState(EState::eChase);
		return true;
	}
	return false;
}

// �ǐՏ�Ԃ��瑼�̏�Ԃֈڍs�̏������`�F�b�N
bool CCollector::ChangeChaseToOther()
{
	// �v���C���[�N���X�擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	CVector targetPos = player->Position();
	// �v���C���[���G���A�O���A���ɉ�������������Ă���ꍇ
	// �ǂ�������̂���߂�
	if (player->AreaOutX() ||
		player->GetStickCollector())
	{
		ChangeState(EState::eIdle);
		return true;
	}
	// �v���C���[�������Ȃ��Ȃ�����A����������Ԃֈڍs
	if (!IsLookPlayer())
	{
		// ���������ʒu�Ƀm�[�h��z�u
		mpLostPlayerNode->SetPos(targetPos);
		mpLostPlayerNode->SetEnable(true);
		ChangeState(EState::eLost);
		mStateStep = 0;
		return true;
	}
	return false;
}

// �ҋ@���
void CCollector::UpdateIdle()
{
	mMoveSpeed = CVector::zero;
	// ������B��������ǐՂ�
	if (ChangeChase())
	{
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
	// ������B��������ǐՂ�
	if (ChangeChase())
	{
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
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	CVector targetPos = player->Position();
	// ���̏�Ԃֈڍs���邩�`�F�b�N
	if (ChangeChaseToOther())
	{
		// �ڍs����Ȃ珈�����Ȃ�
		return;
	}

	// �v���C���[�ɍU���ł��鋗���Ȃ��
	if (CanAttackPlayer(ATTACK_RANGE))
	{
		// ����������ɂ��Ă��Ȃ���΍U����Ԃ�
		if (!player->GetStickCollector())
		{
			ChangeState(EState::eAttackStart);
		}
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
		if (navMgr->Navigate(mpNavNode, playerNode, mpMoveRoute))
		{
			// ���g�̃m�[�h����v���C���[�̃m�[�h�܂Ōq�����Ă�����A
			// �ړ�����ʒu�����̃m�[�h�̈ʒu�ɐݒ�
			targetPos = mpMoveRoute[1]->GetPos();
			// �����Ɠ����ʒu���ړI�n�̏ꍇ�ɑΏ�
			for (int i = 2; i; i++)
			{
				CVector vec = targetPos - Position();
				vec.Y(0.0f);
				// �x�N�g���̒�����2����v�Z
				float moveDist = vec.LengthSqr();
				// ������0�ł͂Ȃ��ꍇ
				if (!moveDist == 0.0f)
				{
					// ���[�v�I��
					break;
				}
				// ������0�Ȃ�
				else
				{
					// �T�C�Y�Ɠ����ɂȂ�����Ō�̗v�f���w�肵�ă��[�v�I��
					if (i == mpMoveRoute.size())
					{
						targetPos = mpMoveRoute[mpMoveRoute.size() - 1]->GetPos();
						break;
					}
					// �ŒZ�o�H�̎��̃m�[�h���w��
					targetPos = mpMoveRoute[i]->GetPos();
				}
			}
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

		// �X�e�b�v1�F�v���C���[�����������ʒu�܂ňړ�
	case 1:
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

// �S�~���W�Ԃɖ߂鏈��
void CCollector::UpdateReturn()
{

	switch (mStateStep)
	{
		// �X�e�b�v0�F�ړ��A�j���[�V�������Đ�
	case 0:
		// �S�~�܂������Ă��Ȃ��Ȃ�
		if (!mIsBag)
		{
			ChangeAnimation((int)EAnimType::eMove);
		}
		// �S�~�܂������Ă���Ȃ�
		else
		{
			ChangeAnimation((int)EAnimType::eMove_Bag);
		}
		mStateStep++;
		break;

		// �X�e�b�v1�F�����̎�����ƂԂ���܂ňړ��𑱂���
	case 1:
		// ������̍��W�ֈړ�
		if (MoveTo(mpOwner->Position(), GetBaseMoveSpeed(), ROTATE_SPEED))
		{

		}
		break;
	}
}

// �U���J�n
void CCollector::UpdateAttackStart()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F�U���̊J�n
	case 0:
		// �U���J�n�A�j���[�V�������Đ�
		ChangeAnimation((int)EAnimType::eAttack_Start);
		mStateStep++;
		break;

		// �X�e�b�v1�F������̈ړ���ݒ�
	case 1:
		// �A�j���[�V������60%�ȏ�i�s������
		if (GetAnimationFrameRatio() >= 0.6f)
		{
			// ������̈ړ���ݒ�
			mMoveSpeedY = GetJumpSpeed();
			// �U���J�n
			AttackStart();
			mStateStep++;
		}
		break;

		// �X�e�b�v2�F�A�j���[�V�������I��������U������
	case 2:
		// �v���C���[�̍��W
		CPlayerBase * player = CPlayerBase::Instance();
		CVector targetPos = player->Position();
		// �v���C���[�̕����ֈړ�
		if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
		{

		}
		// �U���J�n�A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �U�������Ȃ�
			if (mIsAttackSuccess)
			{
				// �U�����i�����j��
				ChangeState(EState::eAttackTrue);
			}
			// ���s�Ȃ�
			else
			{
				// �U�����i���s�j��
				ChangeState(EState::eAttackFalse);
			}
		}
		break;
	}
}

// �U�����i�����j
void CCollector::UpdateAttackTrue()
{
	// ���W���v���C���[�̑O�ɌŒ�
	CPlayerBase* player = CPlayerBase::Instance();
	CVector playerForward = player->VectorZ();
	CVector playerUp = player->VectorY();
	// �Œ肷����W�����߂�
	CVector lockPos = player->Position() + playerForward * ATTACK_SUCCESS_OFFSET_POSZ +
		playerUp * ATTACK_SUCCESS_OFFSET_POSY;
	Position(lockPos);
	// �����������v���C���[�̕����ɂ���
	CVector lookVec = player->Position() - Position();
	lookVec.Y(0.0f);
	Rotation(CQuaternion::LookRotation(lookVec));

	switch (mStateStep)
	{
		// �X�e�b�v0�F�A�j���[�V�����Đ�
	case 0:
	{
		// �Փ˔�����I�t
		SetEnableCol(false);
		// �d�͂��|���Ȃ�
		mIsGravity = false;
		// �U�������A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eAttack_True);
		mStateStep++;
		break;
	}

		// �X�e�b�v1�F���̃A�j���[�V�����Đ�
	case 1:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �S�~�܊l���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eGetBag);
			mStateStep++;
		}
		break;

		// �X�e�b�v2�F�S�~�܂���Ɏ���
	case 2:
		// �A�j���[�V������200�t���[���ȍ~�Ȃ�
		if (GetAnimationFrame() >= 200.0f)
		{
			CTrashPlayer* trashPlayer = dynamic_cast<CTrashPlayer*>(player);
			// �v���C���[���ʏ�̃S�~�܂��������Ă���ꍇ
			if (trashPlayer->GetTrashBag() > 0)
			{
				int power = GetPower();
				// �S�~�܂̐����p���[��菭�Ȃ��ꍇ
				if (trashPlayer->GetTrashBag() < power)
				{
					// �v���C���[�̎������Ɠ����Ƀp���[��ݒ�
					power = trashPlayer->GetTrashBag();
				}
				// �S�~�܂̐����p���[�����炷
				trashPlayer->SetTrashBag(-power);
				// �������������̎������ɉ�����
				SetTrashBag(power);
				// �S�[���h����Ȃ�
				mpTrashBag->SetGold(false);
				// �S�~�܂������Ă���
				SetHaveBag(true);
			}
			// �v���C���[���ʏ�̃S�~�܂��������Ă��Ȃ�����
			// �S�[���h�̃S�~�܂��������Ă���ꍇ
			else if (trashPlayer->GetGoldTrashBag() > 0)
			{
				int power = GetPower();
				// �S�[���h�̃S�~�܂̐����p���[��菭�Ȃ��ꍇ
				if (trashPlayer->GetGoldTrashBag() < power)
				{
					// �v���C���[�̎������Ɠ����Ƀp���[��ݒ�
					power = trashPlayer->GetGoldTrashBag();
				}
				// �S�[���h�̃S�~�܂̐����p���[�����炷
				trashPlayer->SetGoldTrashBag(-power);
				// �������������̎������ɉ�����
				SetGoldTrashBag(power);
				// �S�[���h�ł���
				mpTrashBag->SetGold(true);
				// �S�~�܂������Ă���
				SetHaveBag(true);
			}
			mStateStep++;
		}
		break;

		// �X�e�b�v3�F�U���I����Ԃ�
	case 3:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �Փ˔�����I��
			SetEnableCol(true);
			// �d�͂��|����
			mIsGravity = true;
			// �U���I����Ԃ�
			ChangeState(EState::eAttackEnd);
			// �U���I��
			AttackEnd();
		}
		break;
	}
}

// �U�����i���s�j
void CCollector::UpdateAttackFalse()
{
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// �U���������ɕς���Ă�����
	if (mIsAttackSuccess)
	{
		// �U��������Ԃ�
		ChangeState(EState::eAttackTrue);
		return;
	}

	// �����̑O����ڕW�n�_�ɐݒ�
	CVector targetPos = Position() + VectorZ() * 10.0f;
	// �ڕW�n�_�̕����ֈړ�
	if (MoveTo(targetPos, GetBaseMoveSpeed(), ROTATE_SPEED))
	{

	}

	switch (mStateStep)
	{
		// �X�e�b�v0�F�A�j���[�V�����Đ�
	case 0:
		// �U�����s�A�j���[�V�����Đ�
		ChangeAnimation((int)EAnimType::eAttack_False);
		mStateStep++;
		break;

		// �X�e�b�v1�F���̃A�j���[�V�����Đ�
	case 1:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �U�����s�������A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eAttack_False_Fall);
			mStateStep++;
		}
		break;

		// �X�e�b�v2�F�S�~�܂���Ɏ����U���I����Ԃ�
	case 2:
		// �ڒn������
		if (mIsGrounded)
		{
			// �U���I����Ԃ�
			ChangeState(EState::eAttackEnd);
			// �U���I��
			AttackEnd();
		}
		break;
	}
}

// �U���I��
void CCollector::UpdateAttackEnd()
{
	switch (mStateStep)
	{
	case 0:
	{
		// �U�������Ȃ�
		if (mIsAttackSuccess)
		{
			// �S�~�܊l���I���A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eGetBag_End);
		}
		// ���s�Ȃ�
		else
		{
			// �����オ��A�j���[�V�����Đ�
			ChangeAnimation((int)EAnimType::eAttack_False_StandUp);
		}
		// �U�������������������Z�b�g
		mIsAttackSuccess = false;
		mStateStep++;
		break;
	}

	case 1:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �S�~�܂������Ă���
			// �e���L���̏ꍇ
			if (mIsBag&&
				mpOwner->IsEnable())
			{
				// �S�~���W�Ԃɖ߂��Ԃ�
				ChangeState(EState::eReturn);
			}
			// �����Ă��Ȃ��Ȃ�
			else
			{
				// �ǐՏ�Ԃ�
				ChangeState(EState::eChase);
			}
		}
		break;
	}
}

// ���S�̍X�V����
void CCollector::UpdateDeath()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F���S�A�j���[�V�������Đ�
	case 0:
		// ������̈ړ����[���ɂ���
		mMoveSpeedY = 0.0f;
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
		DropTrashBag(1, Position(), VectorZ(), VectorX(), TRASH_BAG_OFFSET_POS);
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
		// �X�e�b�v4�F�����ɂ���
	case 4:
		SetOnOff(false);
		break;
	}
}

// ���S����
void CCollector::Death()
{
	// ���S��Ԃ�
	ChangeState(EState::eDeath);
}

// �S�~�܂������Ă��邩��؂�ւ���
void CCollector::SetHaveBag(bool isHave)
{
	// �����Ȃ珈�����Ȃ�
	if (mIsBag == isHave) return;

	mIsBag = isHave;
	mpTrashBag->SetEnable(isHave);
	mpTrashBag->SetShow(isHave);
}


// �U������
bool CCollector::IsAttacking() const
{
	return mIsAttacking;
}

// �U���J�n
void CCollector::AttackStart()
{
	// �x�[�X�N���X�̍U���J�n�������Ăяo��
	CXCharacter::AttackStart();
	// �U����
	mIsAttacking = true;
}

// �U���I��
void CCollector::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CXCharacter::AttackEnd();

	// �U�����łȂ�
	mIsAttacking = false;
	// �d�͂��|����
	mIsGravity = true;
	// �v���C���[�̃N���X���擾
	CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
	// ���Ă��������̃|�C���^���擾
	CCollector* stickCollector = player->GetStickCollectorPointer();
	// ���Ă��������ƈ�v�����
	if (this == stickCollector)
	{
		// ���Ă���̂�����
		player->SetStickCollector(false);
	}
}

// �U��������������
bool CCollector::GetAttackSuccess() const
{
	return mIsAttackSuccess;
}

// �_���[�W���󂯂�
void CCollector::TakeDamage(int damage, CObjectBase* causer)
{
	CCharaStatusBase::TakeDamage(damage, causer);
}

// ��Ԑ؂�ւ�
void CCollector::ChangeState(EState state)
{
	// ���ɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	// �U�����ɍU���֘A�ȊO�̏�Ԃɕς��Ƃ�
	// �U���I���������Ă�
	if (IsAttacking() &&
		state != EState::eAttackStart &&
		state != EState::eAttackTrue &&
		state != EState::eAttackFalse &&
		state != EState::eAttackEnd)
	{
		AttackEnd();
		// �U�������������������Z�b�g
		mIsAttackSuccess = false;
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
	case EState::eAttackTrue:		return "�U�����i�����j";
	case EState::eAttackFalse:		return "�U�����i���s�j";
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
	case EState::eAttackTrue:		return CColor::magenta;
	case EState::eAttackFalse:		return CColor::magenta;
	case EState::eAttackEnd:		return CColor::magenta;
	}
	return CColor::white;
}
#endif
