#include "CPlayer.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CInput.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "CConnectPointManager.h"
#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "CImage.h"
#include "CFade.h"
#include "Maths.h"

// �̂̔��a�ƍ���
#define BODY_RADIUS 2.5f
#define BODY_HEIGHT 15.0f

// �T�m�R���C�_�̔��a
#define SEARCH_RADIUS 50.0f

// ��̃I�t�Z�b�g���W�Ɖ�]�ƃX�P�[��
#define WAND_OFFSET_POS CVector(-90.0f,8.0f,4.0f)
#define WAND_OFFSET_ROT CVector(0.0f,0.0f,90.0f)
#define WAND_SCALE	3.0f

// �^�[�Q�b�g����Ƃ��̎��_�ƃI�u�W�F�N�g�̉�ʋ����̍ő�
#define TARGET_MAX_DISTANCE (WINDOW_HEIGHT / 2)

// �^�[�Q�b�g�ꏊ�̉摜�̃X�P�[��
#define TARGET_POINT_IMG_SCALE 0.1f

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Adventurer\\AdventurerAnim\\"

// �A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",								true,	0.0f,	1.0f},	// T�|�[�Y
	{ANIM_PATH"Idle.x",					true,	118.0f,	1.0f},	// �ҋ@
	{ANIM_PATH"Idle_Wand.x",			true,	221.0f,	1.0f},	// �񎝂��ҋ@
	{ANIM_PATH"Run.x",					true,	48.0f,	1.0f},	// ����
	{ANIM_PATH"Run_Wand.x",				true,	43.0f,	1.0f},	// �񎝂�����
	{ANIM_PATH"Jump_Start.x",			false,	40.0f,	2.0f},	// �W�����v�J�n
	{ANIM_PATH"Jump.x",					true,	40.0f,	1.0f},	// �W�����v��
	{ANIM_PATH"Jump_End.x",				false,	11.0f,	2.0f},	// �W�����v�I��
	{ANIM_PATH"Attack_Wand.x",			false,	61.0f,	1.5f},	// ��U��
	{ANIM_PATH"Swing.x",				true,	70.0f,	1.0f},	// �X�C���O
	{ANIM_PATH"Swing_End_Start.x",		false,	60.0f,	2.0f},	// �X�C���O�I���̊J�n
	{ANIM_PATH"Swing_End.x",			true,	40.0f,	1.0f},	// �X�C���O�I����
	{ANIM_PATH"Swing_End_End.x",		false,	69.0f,	2.0f},	// �X�C���O�I���̏I��
};

// �^�[�U�����̐��̒���
#define TARZAN_DISTANCE 40.0f
// �������鑬�x
#define DECREASE_SPEED 0.1f
// �������鑬�x
#define INCREASE_SPEED 0.5f

// �R���X�g���N�^
CPlayer::CPlayer()
	: CPlayerBase()
	, CPlayerStatus()
	, mState(EState::eIdle)
	, mIsWand(true)
	, mIsAttacking(false)
	, mpWandPoint(nullptr)
	, mpCenterTarget(nullptr)
	, mRespawnPos(CVector::zero)
{
	// �A�j���[�V�����ƃ��f���̏�����
	InitAnimationModel("Player", &ANIM_DATA);

	// �R���C�_�\�𐶐�
	CreateCol();

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

	// ����쐬
	mpWand = new CWand
	(
		this,
		ETag::eWand
	);

	// �E��̃t���[�����擾���A
	// ��Ƀv���C���[�̉E��̍s����A�^�b�`
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	mpWand->SetAttachMtx(&frame->CombinedMatrix());
	mpWand->Position(WAND_OFFSET_POS);
	mpWand->Rotation(WAND_OFFSET_ROT);
	mpWand->Scale(mpWand->Scale() * WAND_SCALE);

	// �^�[�Q�b�g���Ă���ꏊ�̉摜
	mpTargetPointImg = new CImage
	(
		"UI\\TargetPoint.png"
	);
	mpTargetPointImg->SetSize(mpTargetPointImg->GetSize() * TARGET_POINT_IMG_SCALE);
	// �ŏ��͔�\��
	mpTargetPointImg->SetEnable(false);
	mpTargetPointImg->SetShow(false);
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{
	// �񂪑��݂�����
	if (mpWand != nullptr)
	{
		// ��������������Ă���A�폜
		mpWand->SetOwner(nullptr);
		mpWand->Kill();
	}

	SAFE_DELETE(mpSearchConnectObjCol);
}

// �U������
bool CPlayer::IsAttacking() const
{
	return mIsAttacking;
}

// �U���J�n
void CPlayer::AttackStart()
{
	mIsAttacking = true;
}

// �U���I��
void CPlayer::AttackEnd()
{
	mIsAttacking = false;
}

// �_���[�W���󂯂�
void CPlayer::TakeDamage(int damage, CObjectBase* causer)
{
}

// �X�V
void CPlayer::Update()
{
	// ��O�֗�������
	if (Position().Y() < MAX_UNDER_POS)
	{
		// �A�ҏ�Ԃ�
		ChangeState(EState::eReturn);
	}

	// �ҋ@���A�^�[�U���J�n�́A�ړ��������s��
	if (mState == EState::eIdle ||
		mState == EState::eTarzanStart)
	{
		UpdateMove();
	}

	// �L�[����
	ActionInput();

	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;	// �ҋ@
	case EState::eDamageStart:	UpdateDamageStart();	break;	// ��e�J�n
	case EState::eDamage:		UpdateDamage();			break;	// ��e��
	case EState::eDamageEnd:	UpdateDamageEnd();		break;	// ��e�I��
	case EState::eAttackStart:	UpdateAttackStart();	break;	// �U���J�n
	case EState::eAttack:		UpdateAttack();			break;	// �U����
	case EState::eAttackEnd:	UpdateAttackEnd();		break;	// �U���I��
	case EState::eTarzanStart:	UpdateTarzanStart();	break;	// �^�[�U���J�n
	case EState::eTarzan:		UpdateTarzan();			break;	// �^�[�U����
	case EState::eTarzanEnd:	UpdateTarzanEnd();		break;	// �^�[�U���I��
	case EState::eReturn:		UpdateReturn();			break;	// ��O����߂�
	case EState::eDeath:		UpdateDeath();			break;	// ���S
	}

	CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();

	if (!mIsGrounded &&
		mState != EState::eTarzanStart &&
		mState != EState::eTarzan &&
		mState != EState::eTarzanEnd &&
		connectPointMgr->IsWandConnectAirObject())
	{
		ChangeState(EState::eTarzanStart);
	}

	// ���v���C���[�N���X�̍X�V
	CPlayerBase::Update();

	// ���S�Ɉ�ԋ߂��I�u�W�F�N�g�����߂�
	CenterTarget();
	// �^�[�Q�b�g���̏ꏊ�̉摜�̈ʒu��L���������X�V
	UpdatePointImg();

	// ��̍s����X�V
	mpWand->UpdateMtx();

#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("IsWand:%s\n", mIsWand ? "�����Ă���" : "�����Ă��Ȃ�");
	CDebugPrint::Print("ConnectObj:%d\n", mConnectObjs.size());
	CDebugPrint::Print("MoveSpeed:%f,%f,%f\n", mMoveSpeed.X(), mMoveSpeed.Y(), mMoveSpeed.Z());
#endif

	// �R�l�N�g�I�u�W�F�N�g�̃��X�g���N���A
	mConnectObjs.clear();
}

// �Փˏ���
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);

	// �{�̃R���C�_�\�Ȃ�
	if (self == mpBodyCol)
	{
		// ���̏ꍇ
		if (other->Layer() == ELayer::eCrushed)
		{
			// �A�ҏ�Ԃ�
			ChangeState(EState::eReturn);
			return;
		}
	}
	// �R�l�N�g�I�u�W�F�N�g�T�m�p�R���C�_�[�Ȃ�
	else if (self == mpSearchConnectObjCol)
	{
		// �R�l�N�g�I�u�W�F�N�g�̏ꍇ
		if (other->Owner()->Tag() == ETag::eConnectObject)
		{
			// �R�l�N�g�I�u�W�F�N�g�N���X���擾
			CConnectObject* obj = dynamic_cast<CConnectObject*>(other->Owner());
			// �˒����ɂ���R�l�N�g�I�u�W�F�N�g�ɒǉ�
			mConnectObjs.push_back(obj);
		}
	}
}

// ��̃|�C���^���擾
CWand* CPlayer::GetWand()
{
	return mpWand;
}

// �����n�_��ݒ�
void CPlayer::SetRespawnPos(CVector respawnPos)
{
	mRespawnPos = respawnPos;
}

// �R���C�_�\�𐶐�
void CPlayer::CreateCol()
{
	// �{�̃R���C�_
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS / Scale().Y(), 0.0f),
		CVector(0.0f, BODY_HEIGHT / Scale().Y(), 0.0f),
		BODY_RADIUS
	);
	// �t�B�[���h,�ǁA�I�u�W�F�N�g�A
	// �X�C�b�`�A�|�[�^���A���X�|�[���n�_�A���X�|�[����������Obj�A�A�C�e���Ƃ����Փ�
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::eSwitch,ELayer::ePortal,
		ELayer::eRespawnArea,ELayer::eCrushed,ELayer::eItem});

	// �R�l�N�g�I�u�W�F�N�g�̒T�m�p�R���C�_
	mpSearchConnectObjCol = new CColliderSphere
	(
		this, ELayer::eConnectSearch,
		SEARCH_RADIUS
	);
	// �R�l�N�g�I�u�W�F�N�g�����Փ�
	mpSearchConnectObjCol->SetCollisionLayers({ ELayer::eObject });
}

// �A�N�V�����̃L�[����
void CPlayer::ActionInput()
{
	// ��������Ă���Ȃ�
	if (mIsWand)
	{
		// ���N���b�N�ōU����
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �^�[�U���I����ԂȂ�
			if (mState != EState::eTarzanEnd)
			{
				// �U���J�n��
				ChangeState(EState::eAttackStart);
			}
			else
			{			
				// ���_�̒��S�ɋ߂��^�[�Q�b�g������Ȃ�
				if (mpCenterTarget != nullptr)
				{
					// �ڑ����̊Ǘ��N���X
					CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
					// �ڑ��I�u�W�F�N�g�̃^�O���󒆂Ȃ�
					if (mpCenterTarget->GetConnectObj()->GetConnectObjTag() == EConnectObjTag::eAir)
					{
						// �ڑ�����L��
						connectPointMgr->EnableConnect(mpCenterTarget);
						// �^�[�U������Ԃ�
						ChangeState(EState::eTarzan);
					}
					// �����łȂ��Ȃ�
					else
					{
						// �U���J�n��
						ChangeState(EState::eAttackStart);
					}
				}
			}
		}
		// ���N���b�N��������Ă��Ȃ��Ȃ�
		if (!CInput::Key(VK_LBUTTON))
		{
			// �ڑ����Ǘ��N���X
			CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
			// ��̐ڑ�������
			connectPointMgr->DisableConnect(connectPointMgr->GetConnectWandTarget());

			// �^�[�U����ԂȂ�
			if (mState == EState::eTarzanStart ||
				mState == EState::eTarzan)
			{
				// �d�̓I��
				mIsGravity = true;
				// �^�[�U���I����Ԃ�
				ChangeState(EState::eTarzanEnd);
			}
			// �U�����
			else if (mState == EState::eAttackStart ||
				mState == EState::eAttack)
			{
				// �U���I����
				ChangeState(EState::eAttackEnd);
			}
		}
	}

#if _DEBUG
	if (CInput::PushKey('G'))
	{
		mIsGravity = !mIsGravity;
	}
#endif
}

// �ҋ@���
void CPlayer::UpdateIdle()
{
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
			// ��������Ă���ꍇ
			if (mIsWand)
			{
				// �񎝂��ړ��A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation((int)EAnimType::eMove_Wand);
			}
			// ��������Ă��Ȃ��ꍇ
			else
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
			// ��������Ă���ꍇ
			if (mIsWand)
			{
				// �񎝂��ҋ@�A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation((int)EAnimType::eIdle_Wand);
			}
			// ��������Ă��Ȃ��ꍇ
			else
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

// �U���J�n
void CPlayer::UpdateAttackStart()
{
	switch (mStateStep)
	{
		// �A�j���[�V�����ύX
	case 0:
		mMoveSpeed = CVector::zero;
		// �U���J�n�A�j���[�V����
		ChangeAnimation((int)EAnimType::eAttack);
		mStateStep++;
		break;

		// �L���������_�̕����֌�����
	case 1:
	{
		CCamera* camera = CCamera::CurrentCamera();
		// �������������߂�
		CVector vec = camera->GetEyeVec();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		mStateStep++;
		break;
	}

		// ���̏�Ԃ�
	case 2:
		// �U������Ԃ�
		ChangeState(EState::eAttack);
		break;
	}
}

// �U����
void CPlayer::UpdateAttack()
{
	switch (mStateStep)
	{
		// �A�j���[�V������20�t���[���ȍ~�Ȃ�
	case 0:
		if (GetAnimationFrame() > 20.0f)
		{
			// ���_�̒��S�ɋ߂��^�[�Q�b�g������Ȃ�
			if (mpCenterTarget != nullptr)
			{
				// �ڑ����̊Ǘ��N���X
				CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
				// �ڑ�����L��
				connectPointMgr->EnableConnect(mpCenterTarget);
			}
			// ����
			mStateStep++;
		}
		break;

		// ���̏�Ԃ�
	case 1:
		// �U���I����
		ChangeState(EState::eAttackEnd);
		break;
	}
}

// �U���I��
void CPlayer::UpdateAttackEnd()
{
	switch (mStateStep)
	{
		// �A�j���[�V������40�t���[���ȍ~�Ȃ玟��
	case 0:
		if (GetAnimationFrame() > 40.0f)
		{
			mStateStep++;
		}
		break;

		// �A�j���[�V�������I�������玟��
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;

		// ���̏�Ԃ�
	case 2:
		// �ҋ@��Ԃ�
		ChangeState(EState::eIdle);
		break;
	}
}

// �^�[�U���J�n
void CPlayer::UpdateTarzanStart()
{
	switch (mStateStep)
	{
		// 20�t���[���i�s������
	case 0:
		mElapsedTime++;
		if (mElapsedTime > 20.0f)
		{
			// �ڑ����Ǘ��N���X
			CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
			// ��̐ڑ����̒�����ݒ�
			connectPointMgr->SetConnectDistance();
			mStateStep++;
		}
		break;
		// �^�[�U������
	case 1:

		ChangeState(EState::eTarzan);
		break;
	}
}

// �^�[�U����
void CPlayer::UpdateTarzan()
{
	switch (mStateStep)
	{
		// �X�C���O�A�j���[�V�����ɐ؂�ւ���
	case 0:
		mMoveSpeed = CVector::zero;
		mMoveSpeedY = 0.0f;
		// �X�C���O�A�j���[�V�����ɐ؂�ւ�
		ChangeAnimation((int)EAnimType::eSwing);
		mStateStep++;
		break;

		// �X�C���O����
	case 1:
		// �ڑ����Ǘ��N���X
		CConnectPointManager * connectPointMgr = CConnectPointManager::Instance();
		// �ڒn������
		if (mIsGrounded)
		{
			// �d�̓I��
			mIsGravity = true;
			// �ҋ@��Ԃ�
			ChangeState(EState::eIdle);
			return;
		}
		// �^�[�Q�b�g
		CConnectTarget* target = connectPointMgr->GetConnectWandTarget();
		// �^�[�Q�b�g��null�Ȃ�d�̓I��
		if (target == nullptr)
		{
			// �d�̓I��
			mIsGravity = true;
			return;
		}
		// �d�̓I�t
		mIsGravity = false;
		// �^�[�Q�b�g�̍��W
		CVector targetPos = target->Position();
		// �v���C���[�̍��W
		CVector playerPos = Position();

		// �^�[�Q�b�g����v���C���[�̕���
		CVector dir = playerPos - targetPos;
		dir.Normalize();

		// �d�͂̕���
		CVector gravity = CVector(0.0f, -GRAVITY, 0.0f);
		// ������������d�͂����O
		gravity = gravity - dir * gravity.Dot(dir);

		// �ړ�����
		CVector moveDir = CalcMoveVec();

		// ���͂���̐����ʂɓ��e
		moveDir = dir.Cross(moveDir.Cross(dir));
		moveDir.Normalize();

		// �ړ����x
		mMoveSpeed += moveDir * INCREASE_SPEED * Times::DeltaTime();
		// �d�͂�������
		// �U��؂鎞�̌����Ɩ߂鎞�̉����̂���
		mMoveSpeed += gravity;

		// �������������Ă���
		mMoveSpeed *= (1.0f - DECREASE_SPEED * Times::DeltaTime());

		// �������̑��x���폜
		// �^�[�Q�b�g�̐^���ł̋}�Ȍ�����h������
		mMoveSpeed -= dir * mMoveSpeed.Dot(dir);
		// �v���C���[���W�ɒǉ�
		playerPos += mMoveSpeed;

		// �V�����v���C���[���W�ւ̕���
		dir = playerPos - targetPos;
		dir.Normalize();
		// �v���C���[�̍��W���������̋����ɕۂ�
		playerPos = targetPos +
			dir * connectPointMgr->GetConnectDistance();

		// ���W��ݒ�
		Position(playerPos);
		break;
	}
}

// �^�[�U���I��
void CPlayer::UpdateTarzanEnd()
{
	switch (mStateStep)
	{
		// �ڑ������������
	case 0:
	{
		// �^�[�U������̃W�����v�A�j���[�V�����ɐ؂�ւ�
		ChangeAnimation((int)EAnimType::eSwing_End_Start);
		// �W�����v���x��ݒ�
		mMoveSpeedY = GetJumpSpeed() * Times::DeltaTime();

		mStateStep++;
		break;
	}
	// �^�[�U������̃W�����v���A�j���[�V�����ɐ؂�ւ�
	case 1:
		// �A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			ChangeAnimation((int)EAnimType::eSwing_End);
			mStateStep++;
		}
		// ���̑O�ɒn�ʂɕt�����玟��
		else if (mIsGrounded)
		{
			mStateStep++;
		}
		break;
		// �^�[�U���I���A�j���[�V�����ɐ؂�ւ�
	case 2:
		if (mIsGrounded)
		{
			mMoveSpeed = CVector::zero;
			ChangeAnimation((int)EAnimType::eSwing_End_End);
			mStateStep++;
		}
		break;
		// �A�j���[�V�������I��������ҋ@��
	case 3:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// ��O����߂�����
void CPlayer::UpdateReturn()
{
	switch (mStateStep)
	{
		// �t�F�[�h�A�E�g���J�n
	case 0:
		CFade::FadeOut();
		mStateStep++;
		break;
		
		// �t�F�[�h�A�E�g���I�������玟��
	case 1:
		if (!CFade::IsFading())
		{
			mStateStep++;
		}
		break;

		// ���W��ݒ�
	case 2:
		// ���X�|�[���n�_�ɐݒ�
		Position(mRespawnPos);
		// �ړ����Ȃ�
		mMoveSpeed = CVector::zero;
		// ��������Ă�����
		if (mIsWand)
		{
			// �񎝂��ҋ@�A�j���[�V����
			ChangeAnimation((int)EAnimType::eIdle_Wand);
		}
		// �����ĂȂ��Ȃ�
		else
		{
			// �ҋ@�A�j���[�V����
			ChangeAnimation((int)EAnimType::eIdle);
		}
		mStateStep++;
		break;
		
		// �t�F�[�h�C�����J�n
	case 3:
		CFade::FadeIn();
		mStateStep++;
		break;
		// �t�F�[�h�C�����I��������ҋ@��Ԃ�
	case 4:
		if (!CFade::IsFading())
		{
			// �ҋ@��Ԃ�
			ChangeState(EState::eIdle);
		}
		break;
	}
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
	mStateStep = 0;
	mElapsedTime = 0.0f;
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
	case EState::eAttackStart:	return "�U���J�n";		break;
	case EState::eAttack:		return "�U����";		break;
	case EState::eAttackEnd:	return "�U���I��";		break;
	case EState::eTarzanStart:	return "�^�[�U���J�n";	break;
	case EState::eTarzan:		return "�^�[�U����";	break;
	case EState::eTarzanEnd:	return "�^�[�U���I��";	break;
	case EState::eReturn:		return "��O����A�Ғ�";break;
	case EState::eDeath:		return "���S";			break;
	}

	return "�G���[";
}
#endif

// ���_�̒��S�Ɉ�ԋ߂��I�u�W�F�N�g�����߂�
void CPlayer::CenterTarget()
{
	// �ŒZ����
	float minDist = -1.0f;
	// ���S�Ɉ�ԋ߂��^�[�Q�b�g
	CConnectTarget* nearTarget = nullptr;

	// ��ʂ̒��S�����߂�
	CVector2 screenCenter = CVector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	// ���X�g�̑S�ẴI�u�W�F�N�g
	for (const auto& obj : mConnectObjs)
	{
		// �I�u�W�F�N�g�����E���ɂȂ��Ȃ玟��
		CVector eyeVec = CCamera::CurrentCamera()->GetEyeVec();
		CVector objVec = obj->Position() - Position();
		float dot = eyeVec.Dot(objVec);
		if (dot < 0.0f) continue;
		
		// �I�u�W�F�N�g�����S�Ẵ^�[�Q�b�g
		for (const auto& target : obj->GetTargets())
		{
			// �ڑ����̊Ǘ��N���X
			CConnectPointManager* pointMgr = CConnectPointManager::Instance();
			// �^�[�Q�b�g��������ꏊ�ɂȂ��Ȃ玟��
			if (pointMgr->RayTarget(target->Position())) continue;

			// �X�N���[�����W�ɕϊ�
			CVector2 screenPos = CCamera::CurrentCamera()->WorldToScreenPos(target->Position());

			// ���S�����2��̒��������߂�
			float dist = (screenPos - screenCenter).LengthSqr();

			// �ŒZ�������s���l���A���߂������̕����Z���ꍇ
			if (minDist == -1.0f || minDist > dist)
			{
				// �ŒZ�������X�V
				minDist = dist;
				// �^�[�Q�b�g��ݒ�
				nearTarget = target;
			}
		}
	}

	// �ŒZ�������^�[�Q�b�g���鎞�̉�ʋ����̍ő�l��2���蒷�����
	if (minDist > TARGET_MAX_DISTANCE * TARGET_MAX_DISTANCE)
	{
		// null��ݒ�
		mpCenterTarget = nullptr;
	}
	// �����łȂ����
	else
	{
		// ��ԋ߂��^�[�Q�b�g��ݒ�
		mpCenterTarget = nearTarget;
	}
}

// �^�[�Q�b�g���̉摜�̈ʒu��L�������̍X�V
void CPlayer::UpdatePointImg()
{
	// ���S�ɋ߂��I�u�W�F�N�g������ꍇ
	if (mpCenterTarget != nullptr)
	{
		// �^�[�Q�b�g���̏ꏊ��\��
		mpTargetPointImg->SetEnable(true);
		mpTargetPointImg->SetShow(true);
		// ���W�����߂�
		CVector2 screenPos = CCamera::CurrentCamera()->WorldToScreenPos(mpCenterTarget->Position());
		// �摜�̃T�C�Y���擾
		float imgSizeX = mpTargetPointImg->GetSize().X();
		float imgSizeY = mpTargetPointImg->GetSize().Y();
		// �T�C�Y�̔��������Z
		screenPos.X(screenPos.X() - imgSizeX / 2);
		screenPos.Y(screenPos.Y() - imgSizeY / 2);
		// ���W��ݒ�
		mpTargetPointImg->SetPos(screenPos);
	}
	// �Ȃ��Ȃ�
	else
	{
		// �^�[�Q�b�g���̏ꏊ���\��
		mpTargetPointImg->SetEnable(false);
		mpTargetPointImg->SetShow(false);
	}
}