#include "CSpellBase.h"
#include "CModel.h"
#include "CSpellCaster.h"

// �R���X�g���N�^
CSpellBase::CSpellBase(ESpellElementalType elemental, ESpellShapeType shape,
	CObjectBase* owner, CObjectBase* target, float deleteTime)
	: CObjectBase(ETag::eSpell, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, CSpellStatus(elemental, shape)
	, mpModel(nullptr)
	, mpOwner(owner)
	, mpTarget(target)
	, mMoveDir(CVector::zero)
	, mDeleteTime(deleteTime)
	, mpSpellCol(nullptr)
{
	// �e�ɂ��Ă���
	SetParent(owner);
}

// �f�X�g���N�^
CSpellBase::~CSpellBase()
{
	SAFE_DELETE(mpSpellCol);
}

// �X�V
void CSpellBase::Update()
{
	switch (mState)
	{
	case CSpellBase::EState::eIdle:			UpdateIdle();		break;
	case CSpellBase::EState::eShooting:		UpdateShooting();	break;
	}

	// �ړ�
	Position(Position() + mMoveSpeed);
}

// �`��
void CSpellBase::Render()
{
	if (mpModel != nullptr)
	{
		mpModel->Render(Matrix());
	}
}

// �Փˏ���
void CSpellBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpSpellCol)
	{
		// ���肪�n�`�Ȃ�
		if (other->Layer() == ELayer::eGround ||
			other->Layer() == ELayer::eWall ||
			other->Layer() == ELayer::eObject)
		{
			// �폜
			Kill();
		}
		// ���肪�v���C���[�Ȃ�
		else if (other->Layer() == ELayer::ePlayer)
		{
			// �폜
			Kill();

#if _DEBUG
			other->Owner()->AddHitCount();
#endif
		}
		// ���肪�G�Ȃ�
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �폜
			Kill();
			
#if _DEBUG
			other->Owner()->AddHitCount();
#endif
		}
		// ���肪�U������Ȃ�
		else if (other->Layer() == ELayer::eAttackCol)
		{
			CSpellBase* spell = dynamic_cast<CSpellBase*>(other->Owner());
			// �����̎����傪�Ⴄ�Ȃ�
			if (spell->mpOwner != mpOwner)
			{
				// �폜
				TakeDamage(spell->GetSpellStatus().power, spell);
			}
		}
		// ���肪�h�䔻��Ȃ�
		else if (other->Layer() == ELayer::eDefenseCol)
		{
			CSpellBase* spell = dynamic_cast<CSpellBase*>(other->Owner());
			// �����̎����傪�Ⴄ�Ȃ�
			if (spell->mpOwner != mpOwner)
			{
				// �폜
				TakeDamage(spell->GetSpellStatus().power, spell);
			}
		}
	}
}

// �����̃^�[�Q�b�g��ݒ�
void CSpellBase::SetTarget(CObjectBase* target)
{
	mpTarget = target;
}

// �����̎������ݒ�
void CSpellBase::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ���˂����X�^�[�g
void CSpellBase::Restart()
{
	mState = EState::eShooting;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// �ҋ@���̍X�V
void CSpellBase::UpdateIdle()
{
}

// ���˒��̍X�V
void CSpellBase::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	// ���ł���܂ł̎��Ԃ��o�߂�����폜
	if (mElapsedTime > mDeleteTime)
	{
		Kill();
	}
}

// �ڕW�ւ̕��������߂�
CVector CSpellBase::TargetDir()
{	
	// �ڕW�ւ̕���
	return mpTarget->Position() - Position();
}

// �ϋv�����炷(�ŏ�����0�̎����̓_���[�W�Ŕj������Ȃ�)
void CSpellBase::TakeDamage(int num, CSpellBase* attacker)
{
	// �ϋv��0�Ȃ珈�����Ȃ�
	if (mSpellStatus.hp == 0) return;

	// ����̃X�e�[�^�X
	SpellStatus status = attacker->GetSpellStatus();

	// ����̌`
	switch (status.shape)
	{
		// ���t���N�^�[�̏ꍇ����
	case ESpellShapeType::eReflector:
	{
		// �r���N���X���擾
		CSpellCaster* caster = dynamic_cast<CSpellCaster*>(mpOwner);
		// �ڕW��ΐ푊��ɐݒ�
		attacker->SetTarget(caster->GetOpponent());
		// �������ݒ�
		attacker->SetOwner(mpOwner);
		// �U���͂�����
		attacker->AddPower(REFLECTOR_POWER_UP);
		// �ϋv������
		attacker->AddHp(REFLECTOR_HP_UP);
		// ���˂����X�^�[�g
		Restart();
		break;
	}
		// ����ȊO
	default:
		// �����{����K�p
		int damage = num * PowerRatio(mSpellStatus.elemental, status.elemental);
		// �_���[�W�����Z
		mSpellStatus.hp -= damage;
		// �ϋv��0�ȉ��Ȃ�
		if (mSpellStatus.hp <= 0)
		{
			mSpellStatus.hp = 0;
			// �폜
			Kill();
		}
		break;
	}
}

// ��Ԃ�؂�ւ�
void CSpellBase::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;

	// �R���C�_�[������Ȃ�
	if (mpSpellCol != nullptr)
	{
		switch (mState)
		{
			// �ҋ@���̓R���C�_�[�͖�����
			// �e�ɂ��Ă���
		case CSpellBase::EState::eIdle:
			mpSpellCol->SetEnable(false);
			SetParent(mpOwner);
			break;
			// ���ˎ��͗L����
			// �e�ɂ��Ă����Ȃ�
		case CSpellBase::EState::eShooting:
			mpSpellCol->SetEnable(true);
			SetParent(nullptr);
			break;
		}
	}
}
