#include "CPlayer.h"
#include "CColliderCapsule.h"
#include "CInput.h"
#include "CImage.h"
#include "CFade.h"
#include "CBall.h"
#include "CGameCamera2.h"

// �̂̔��a�ƍ���
#define BODY_RADIUS 2.5f
#define BODY_HEIGHT 15.0f

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Adventurer\\AdventurerAnim\\"

// �A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",								true,	0.0f,	1.0f},	// T�|�[�Y
};

// �R���X�g���N�^
CPlayer::CPlayer()
	: CPlayerBase()
	, CPlayerStatus()
	, CSpellCaster(this)
	, mState(EState::eIdle)
	, mIsAttacking(false)
{
	// �d�͖���
	mIsGravity = false;
	// �ړ������������Ȃ�
	mIsMoveDir = false;
	// �A�j���[�V�����ƃ��f���̏�����
	//InitAnimationModel("Player", &ANIM_DATA);

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{
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
	// ����̕�������������
	CVector lookPos = mpOpponent->Position();
	lookPos.Y(Position().Y());
	LookAt(lookPos);

	// �ҋ@���́A�ړ��������s��
	if (mState == EState::eIdle)
	{
		UpdateMove();
	}

	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;	// �ҋ@
	case EState::eDamageStart:	UpdateDamageStart();	break;	// ��e�J�n
	case EState::eDamage:		UpdateDamage();			break;	// ��e��
	case EState::eDamageEnd:	UpdateDamageEnd();		break;	// ��e�I��
	case EState::eAttackStart:	UpdateAttackStart();	break;	// �U���J�n
	case EState::eAttack:		UpdateAttack();			break;	// �U����
	case EState::eAttackEnd:	UpdateAttackEnd();		break;	// �U���I��
	case EState::eDeath:		UpdateDeath();			break;	// ���S
	}

	// ���v���C���[�N���X�̍X�V
	CPlayerBase::Update();
	// �����r���N���X�̍X�V
	CSpellCaster::Update();


#if _DEBUG
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("MoveSpeed:%f,%f,%f\n", mMoveSpeed.X(), mMoveSpeed.Y(), mMoveSpeed.Z());
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
		CVector(0.0f, BODY_RADIUS / Scale().Y(), 0.0f),
		CVector(0.0f, BODY_HEIGHT / Scale().Y(), 0.0f),
		BODY_RADIUS
	);
	// �t�B�[���h,�ǁA�I�u�W�F�N�g�ƓG�ƍU���Ƃ����Փ�
	mpBodyCol->SetCollisionLayers({ ELayer::eGround,
		ELayer::eWall,ELayer::eObject,ELayer::eEnemy,
		ELayer::eAttackCol});
}

// �A�N�V�����̃L�[����
void CPlayer::ActionInput()
{
	// ���N���b�N�ōU����
	if (CInput::PushKey(VK_LBUTTON))
	{
		// �U���J�n��
		ChangeState(EState::eAttackStart);
	}

	if (CInput::PushKey('U'))
	{
		// �{�[���������r��
		CastStart(ESpellElementalType::eFire, ESpellShapeType::eBall);
	}
	if (CInput::PushKey('I'))
	{
		// �{���g�������r��
		CastStart(ESpellElementalType::eFire, ESpellShapeType::eBolt);
	}
	if (CInput::PushKey('O'))
	{
		// �u���X�������r��
		CastStart(ESpellElementalType::eFire, ESpellShapeType::eBreath);
	}

	// �X�y�[�X�ŏ�ړ�
	if (CInput::Key(VK_SPACE))
	{
		mMoveSpeedY = GetJumpSpeed() * Times::DeltaTime();
	}
	// �V�t�g�ŉ��ړ�
	else if (CInput::Key(VK_SHIFT))
	{
		mMoveSpeedY = -GetJumpSpeed() * Times::DeltaTime();
	}
	else
	{
		mMoveSpeedY = 0.0f;
	}

}

// �ҋ@���
void CPlayer::UpdateIdle()
{
	// �L�[���͉\
	ActionInput();
}

// �ړ�����
void CPlayer::UpdateMove()
{
	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed = move * GetBaseMoveSpeed() * Times::DeltaTime();

		// �ҋ@��Ԃł���΁A�ړ��A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{	

		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{

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
}

// �U����
void CPlayer::UpdateAttack()
{
}

// �U���I��
void CPlayer::UpdateAttackEnd()
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
	case EState::eDeath:		return "���S";			break;
	}

	return "�G���[";
}
#endif