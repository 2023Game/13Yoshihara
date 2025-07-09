#include "CPlayer.h"
#include "CharaStatusDefine.h"
#include "CColliderSphere.h"
#include "CInput.h"
#include "CImage.h"
#include "CFade.h"
#include "CBall.h"
#include "CGameCamera2.h"
#include "CGaugeUI2D.h"
#include "CTextUI2D.h"
#include "CEnemyManager.h"

// �̂̔��a
#define BODY_RADIUS 4.0f

// �r�������̃I�t�Z�b�g���W
#define SPELL_TEXT_UI_OFFSET_POS CVector(0.0f,WINDOW_HEIGHT * 0.8f, 0.0f)

#define SPELLS {"fire","thunder","wind","ball","bolt","breath","teleport","shield","reflector"}

// �Q�[�W�̃p�X
#define GAUGE_PATH "UI\\gauge.png"
// �Q�[�W�̍��W
#define GAUGE_POS CVector(0.0f,0.0f,0.0f)
// �Q�[�W�̑傫��
#define GAUGE_SIZE 2.0f
// �Q�[�W���m�̊Ԋu
#define GAUGE_DIST 10.0f

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
	, CCastSpellStr(this, ECastType::eQuick, SPELLS, SPELL_TEXT_UI_OFFSET_POS)
	, mState(EState::eIdle)
	, mIsAttacking(false)
{
	// �d�͖���
	mIsGravity = false;
	// �ړ������������Ȃ�
	mIsMoveDir = false;
	// �A�j���[�V�����ƃ��f���̏�����
	//InitAnimationModel("Player", &ANIM_DATA);

	// HP�Q�[�W��ݒ�
	mpHpGauge = new CGaugeUI2D(this, GAUGE_PATH, true, CGaugeUI2D::EGaugeType::eHpGauge);
	mpHpGauge->Size(GAUGE_SIZE);
	mpHpGauge->Position(GAUGE_POS);
	mpHpGauge->SetMaxPoint(GetMaxHp());
	mpHpGauge->SetCurrPoint(GetHp());

	// MP�Q�[�W��ݒ�
	mpMpGauge = new CGaugeUI2D(this, GAUGE_PATH, true, CGaugeUI2D::EGaugeType::eMpGauge);
	mpMpGauge->Size(GAUGE_SIZE);
	mpMpGauge->Position(GAUGE_POS + CVector(0.0f, mpMpGauge->Size().Y() + GAUGE_DIST, 0.0f));
	mpMpGauge->SetMaxPoint(GetMaxMp());
	mpMpGauge->SetCurrPoint(GetMp());

	// �����̕�����e�L�X�g�̎������ݒ�
	mpSpellText->SetOwner(this);

	// �R���C�_�\�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{
	// MP�Q�[�W�����݂�����A�ꏏ�ɍ폜����
	if (mpMpGauge != nullptr)
	{
		mpMpGauge->SetOwner(nullptr);
		mpMpGauge->Kill();
	}
}

// �X�V
void CPlayer::Update()
{
	// ���肪�����
	if (mpOpponent != nullptr)
	{
		// ����̕�������������
		CVector lookPos = mpOpponent->Position();
		lookPos.Y(Position().Y());
		LookAt(lookPos);
	}

	// ���X�Ɍ���
	mMoveSpeed -= mMoveSpeed * DECREASE_MOVE_SPEED;

	// �ҋ@���́A�ړ��������s��
	if (mState == EState::eIdle)
	{
		UpdateMove();
	}

	// �ҋ@���r�����Ȃ�
	if (mState == EState::eIdle ||
		mState == EState::eCast)
	{
		// �r���̃L�[����
		CastInput();
	}

	switch (mState)
	{
	case EState::eIdle:			UpdateIdle();			break;	// �ҋ@
	case EState::eDamageStart:	UpdateDamageStart();	break;	// ��e�J�n
	case EState::eDamage:		UpdateDamage();			break;	// ��e��
	case EState::eDamageEnd:	UpdateDamageEnd();		break;	// ��e�I��
	case EState::eDeath:		UpdateDeath();			break;	// ���S
	}
	// Mp�̍Đ�
	RegeneMp();

	// ���v���C���[�N���X�̍X�V
	CPlayerBase::Update();
	// �r�������w��N���X�̍X�V
	CCastSpellStr::Update();

	// HP�Q�[�W���X�V
	mpHpGauge->SetCurrPoint(GetHp());
	// MP�Q�[�W
	mpMpGauge->SetCurrPoint(GetMp());


#if _DEBUG
	CDebugPrint::Print("PlayerHp:%d\n", GetHp());
	CDebugPrint::Print("PlayerMp:%f\n", GetMp());
	CDebugPrint::Print("PlayerState:%s\n", GetStateStr(mState).c_str());
#endif
}

// �v���C���[���r����Ԃ�
bool CPlayer::IsCastState() const
{
	return mState == EState::eCast ? true : false;
}

// �I�u�W�F�N�g�폜����
void CPlayer::DeleteObject(CObjectBase* obj)
{
	CPlayerBase::DeleteObject(obj);

	// �폜���ꂽ�̂������̃e�L�X�g�Ȃ�
	// �|�C���^����ɂ���
	if (obj == mpSpellText)
	{
		mpSpellText = nullptr;
	}
}

// �R���C�_�\�𐶐�
void CPlayer::CreateCol()
{
	// �{�̃R���C�_
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::ePlayer,
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

// �r���̃L�[����
void CPlayer::CastInput()
{
	switch (mCastType)
	{
		// ��{�r���̃L�[����
	case CCastSpellStr::ECastType::eBasic:
		// �r�����Ȃ�
		if (mState == EState::eCast)
		{
			// �L�[����
			BasicCastInput();
			// �G���^�[�Ŏ����𔭓�
			if (CInput::PushKey(VK_RETURN))
			{
				// �������r��
				CastSpell();
				//�@�ҋ@��
				ChangeState(EState::eIdle);
			}
			// �o�b�N�X�y�[�X�ŕ�����̍폜
			if (CInput::PushKey(VK_BACK))
			{
				DeleteStr();
			}
		}
		// �����łȂ��Ȃ�
		else
		{
			// �G���^�[�ŉr������
			if (CInput::PushKey(VK_RETURN))
			{
				// ��������N���A
				CInput::ClearStr();
				ChangeState(EState::eCast);
			}
		}
		break;
		// �Z�k�r���̃L�[����
	case CCastSpellStr::ECastType::eQuick:
		// �L�[����
		QuickCastInput();
		// �G���^�[�Ŏ����𔭓�
		if (CInput::PushKey(VK_RETURN))
		{
			CastSpell();
		}
		// �o�b�N�X�y�[�X�ŕ�����̍폜
		if (CInput::PushKey(VK_BACK))
		{
			DeleteStr();
		}
		break;
	}
}

// ��{�r���̃L�[����
void CPlayer::BasicCastInput()
{
	// �����ꂽ��������擾���Đݒ�
	BasicCastSpell(CInput::GetInputStr());
}

// �Z�k�r���̃L�[����
void CPlayer::QuickCastInput()
{
	if (CInput::PushKey('U'))
	{
		QuickCastSpell(0);
	}
	if (CInput::PushKey('I'))
	{
		QuickCastSpell(1);
	}
	if (CInput::PushKey('O'))
	{
		QuickCastSpell(2);
	}
	if (CInput::PushKey('J'))
	{
		QuickCastSpell(3);
	}
	if (CInput::PushKey('K'))
	{
		QuickCastSpell(4);
	}
	if (CInput::PushKey('L'))
	{
		QuickCastSpell(5);
	}
	if (CInput::PushKey('M'))
	{
		QuickCastSpell(6);
	}
	if (CInput::PushKey(VK_OEM_COMMA))
	{
		QuickCastSpell(7);
	}
	if (CInput::PushKey(VK_OEM_PERIOD))
	{
		QuickCastSpell(8);
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
	case EState::eDamageStart:	return "��e�J�n";		break;
	case EState::eDamage:		return "��e��";		break;
	case EState::eDamageEnd:	return "��e�I��";		break;
	case EState::eCast:			return "�r����";		break;
	case EState::eDeath:		return "���S";			break;
	}

	return "�G���[";
}
#endif

// �w������ň�Ԋp�x���߂��G�����b�N�I���^�[�Q�b�g�ɐݒ�
void CPlayer::ChangeLockOnTarget(EDirection dir)
{
	// �^�[�Q�b�g�����擾
	std::vector<CEnemy*> targets = CEnemyManager::Instance()->GetTargets();

	switch (dir)
	{
		// �����
	case CPlayer::EDirection::eUp:

		break;
		// ������
	case CPlayer::EDirection::eDown:
		break;
		// ������
	case CPlayer::EDirection::eLeft:
		break;
		// �E����
	case CPlayer::EDirection::eRight:
		break;
	}
}