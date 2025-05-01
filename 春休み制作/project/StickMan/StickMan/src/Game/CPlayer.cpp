#include "CPlayer.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CInput.h"
#include "CWand.h"
#include "CConnectPoint.h"
#include "CConnectPointManager.h"
#include "CConnectObject.h"

// �̂̔��a�ƍ���
#define BODY_RADIUS 2.5f
#define BODY_HEIGHT 15.0f

// �T�m�R���C�_�̔��a
#define SEARCH_RADIUS 100.0f

// ��̃I�t�Z�b�g���W�Ɖ�]�ƃX�P�[��
#define WAND_OFFSET_POS CVector(-90.0f,8.0f,4.0f)
#define WAND_OFFSET_ROT CVector(0.0f,0.0f,90.0f)
#define WAND_SCALE	3.0f

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
	{ANIM_PATH"Jump.x",					false,	51.0f,	1.0f},	// �W�����v
	{ANIM_PATH"Attack_Wand.x",			false,	61.0f,	1.5f},	// ��U��
};

// �R���X�g���N�^
CPlayer::CPlayer()
	: CPlayerBase()
	, CPlayerStatus()
	, mState(EState::eIdle)
	, mIsWand(false)
	, mIsAttacking(false)
	, mpPoint(nullptr)
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

	// �ŏ��͔�\��
	mpWand->SetEnable(false);
	mpWand->SetShow(false);
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
	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;	// �ҋ@
	case EState::eDamageStart:	UpdateDamageStart();	break;	// ��e�J�n
	case EState::eDamage:		UpdateDamage();			break;	// ��e��
	case EState::eDamageEnd:	UpdateDamageEnd();		break;	// ��e�I��
	case EState::eJumpStart:	UpdateJumpStart();		break;	// �W�����v�J�n
	case EState::eJump:			UpdateJump();			break;	// �W�����v��
	case EState::eJumpEnd:		UpdateJumpEnd();		break;	// �W�����v�I��
	case EState::eAttackStart:	UpdateAttackStart();	break;	// �U���J�n
	case EState::eAttack:		UpdateAttack();			break;	// �U����
	case EState::eAttackEnd:	UpdateAttackEnd();		break;	// �U���I��
	case EState::eDeath:		UpdateDeath();			break;
	}

	// �ҋ@���ƃW�����v���́A�ړ��������s��
	if (mState == EState::eIdle ||
		mState == EState::eJumpStart ||
		mState == EState::eJump ||
		mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	// ���v���C���[�N���X�̍X�V
	CPlayerBase::Update();

	// �R�l�N�g�I�u�W�F�N�g�̃��X�g���N���A
	mConnectObjs.clear();

	// ��̍s����X�V
	mpWand->UpdateMtx();

#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("IsWand:%s\n", mIsWand ? "�����Ă���" : "�����Ă��Ȃ�");
#endif
}

// �Փˏ���
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);

	// �R�l�N�g�I�u�W�F�N�g�T�m�p�R���C�_�[�Ȃ�
	if (self == mpSearchConnectObjCol)
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
	// �t�B�[���h,�ǁA�I�u�W�F�N�g�A�X�C�b�`�Ƃ����Փ�
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::eSwitch });

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
			// �ҋ@��ԂȂ�
			if (mState == EState::eIdle)
			{
				// �U���J�n��
				ChangeState(EState::eAttackStart);
			}
			// ����ȊO�̏ꍇ�̓A�j���[�V������ύX�����Ɍq����
			else
			{
				CVector hitPos = CVector::zero;
				CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
				// ���C���Փ˂��邩����
				// �Փ˂�����Փˈʒu�Ɛڑ�
				if (connectPointMgr->Ray(hitPos))
				{
					// �ڑ����𐶐�
					connectPointMgr->CreateConnectPoint(hitPos);
				}
			}
		}
		CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
		// F�Őڑ���Ԃ̃L�����Z��
		if (connectPointMgr->GetWandConnect() && CInput::PushKey('F'))
		{
			connectPointMgr->SetWandConnect(false);
			connectPointMgr->DeleteLastConnectPoint();
		}
	}

	// �ڒn���Ă����
	if (mIsGrounded)
	{
		// �X�y�[�X�ŃW�����v
		if (CInput::PushKey(VK_SPACE))
		{
			// �W�����v�J�n��
			ChangeState(EState::eJumpStart);
		}
	}

	if (CInput::PushKey('L'))
	{
		mIsWand = !mIsWand;
		mpWand->SetEnable(mIsWand);
		mpWand->SetShow(mIsWand);
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

// �W�����v�J�n
void CPlayer::UpdateJumpStart()
{
	switch (mStateStep)
	{
		// �W�����v�J�n
	case 0:
		// �W�����v�A�j���[�V�����ɐ؂�ւ�
		ChangeAnimation((int)EAnimType::eJump);
		// �W�����v�̑��x��ݒ�
		mMoveSpeedY = GetJumpSpeed() * Times::DeltaTime();
		mStateStep++;
		break;

		// ���̏�Ԃ�
	case 1:
		// �W�����v����Ԃ�
		ChangeState(EState::eJump);
		break;
	}
}

// �W�����v��
void CPlayer::UpdateJump()
{
	switch (mStateStep)
	{
		// �n�ʂɕt�����玟�̏�Ԃ�
	case 0:
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
void CPlayer::UpdateJumpEnd()
{
	switch (mStateStep)
	{
		// �ҋ@��Ԃ�
	case 0:
		ChangeState(EState::eIdle);
		break;
	}
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
		// �A�j���[�V������20�t���[���ȍ~�Ȃ玟��
	case 0:
		if (GetAnimationFrame() > 20.0f)
		{
			CVector hitPos = CVector::zero;
			CConnectPointManager* connectPointMgr = CConnectPointManager::Instance();
			// ���C���Փ˂��邩����
			// �Փ˂�����Փˈʒu�Ɛڑ�
			if (connectPointMgr->Ray(hitPos))
			{
				// �ڑ����𐶐�
				connectPointMgr->CreateConnectPoint(hitPos);
			}
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
	case EState::eJumpStart:	return "�W�����v�J�n";	break;
	case EState::eJump:			return "�W�����v��";	break;
	case EState::eJumpEnd:		return "�W�����v�I��";	break;
	case EState::eAttackStart:	return "�U���J�n";		break;
	case EState::eAttack:		return "�U����";		break;
	case EState::eAttackEnd:	return "�U���I��";		break;
	case EState::eDeath:		return "���S";			break;
	}
}
#endif

// ���_�̒��S�Ɉ�ԋ߂��I�u�W�F�N�g�����߂�
CConnectObject* CPlayer::CenterObject()
{
	// ��ʂ̒��S�����߂�
	CVector2 screenCenter = CVector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	// ���X�g�̑S�ẴI�u�W�F�N�g
	for (const auto& obj : mConnectObjs)
	{
		// �X�N���[�����W�ɕϊ�
		CVector2 screenPos = obj->Position();
	}
	return nullptr;
}
