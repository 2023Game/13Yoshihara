#include "CSpellBase.h"
#include "CModel.h"
#include "CSpellCaster.h"
#include "CDamageUI3D.h"
#include "CPlayer.h"

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
			// ������Ȃ珈�����Ȃ�
			if (mpOwner == other->Owner()) return;
			// ���Ƀq�b�g���Ă����珈�����Ȃ�
			if (IsAttackHitObj(other->Owner())) return;

			// �_���[�W�\�L�𐶐�
			DamageText(mSpellStatus.power, mSpellStatus.elemental, this);

			// �U�����q�b�g�����I�u�W�F�N�g�ɒǉ�
			AddAttackHitObj(other->Owner());

#if _DEBUG
			other->Owner()->AddHitCount();
#endif

			// �e���|�[�g�����Ȃ珈�����Ȃ�
			if (mSpellStatus.shape == ESpellShapeType::eTeleport) return;
			// �폜
			Kill();
		}
		// ���肪�G�Ȃ�
		else if (other->Layer() == ELayer::eEnemy)
		{
			// ������Ȃ珈�����Ȃ�
			if (mpOwner == other->Owner()) return;
			// ���Ƀq�b�g���Ă����珈�����Ȃ�
			if (IsAttackHitObj(other->Owner())) return;

			// �_���[�W�\�L�𐶐�
			DamageText(mSpellStatus.power, mSpellStatus.elemental, this);

			// �U�����q�b�g�����I�u�W�F�N�g�ɒǉ�
			AddAttackHitObj(other->Owner());
			
#if _DEBUG
			other->Owner()->AddHitCount();
#endif

			// �e���|�[�g�����Ȃ珈�����Ȃ�
			if (mSpellStatus.shape == ESpellShapeType::eTeleport) return;
			// �폜
			Kill();
		}
		// ���肪�U������Ȃ�
		else if (other->Layer() == ELayer::eAttackCol)
		{
			CSpellBase* spell = dynamic_cast<CSpellBase*>(other->Owner());
			// �����̎����傪�Ⴄ���܂��q�b�g���Ă��Ȃ��Ȃ�
			if (spell->mpOwner != mpOwner &&
				!IsAttackHitObj(other->Owner()))
			{
				// �_���[�W��^����
				spell->TakeDamage(mSpellStatus.power, this);
				// �U�����q�b�g�����I�u�W�F�N�g�ɒǉ�
				AddAttackHitObj(other->Owner());
			}
		}
		// ���肪�h�䔻��Ȃ�
		else if (other->Layer() == ELayer::eDefenseCol)
		{
			CSpellBase* spell = dynamic_cast<CSpellBase*>(other->Owner());
			// �����̎����傪�Ⴄ���܂��q�b�g���Ă��Ȃ��Ȃ�
			if (spell->mpOwner != mpOwner &&
				!IsAttackHitObj(other->Owner()))
			{
				// �_���[�W��^����
				spell->TakeDamage(mSpellStatus.power, this);
				// �U�����q�b�g�����I�u�W�F�N�g�ɒǉ�
				AddAttackHitObj(other->Owner());
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

// �ϋv�����炷(�ŏ�����ϋv0�̎����̓_���[�W�Ŕj������Ȃ�)
void CSpellBase::TakeDamage(int num, CSpellBase* attacker)
{
	// �ϋv��0�Ȃ珈�����Ȃ�
	if (mSpellStatus.hp == 0) return;
	// �U���͂�0�ȉ��Ȃ珈�����Ȃ�
	if (num <= 0) return;

	// �U���҂̃X�e�[�^�X
	SpellStatus attackerStatus = attacker->GetSpellStatus();
	// �_���[�W
	int damage = num;

	// ���g�̌`
	switch (mSpellStatus.shape)
	{
		// ���t���N�^�[�̏ꍇ����
	case ESpellShapeType::eReflector:
	{
		// ��_�Ȃ�
		if (ELEMENTAL_POWER_RATIO <= ElementalPowerRatio(attackerStatus.elemental, mSpellStatus.elemental))
		{
			// �폜
			Kill();
			return;
		}
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
		attacker->Restart();
		break;
	}
		// �V�[���h�̏ꍇ����
	case ESpellShapeType::eShield:
		// �U���҂��폜
		attacker->Kill();
		break;
		// ����ȊO�̏ꍇ
	default:
		break;
	}

	// �����{����K�p
	damage *= ElementalPowerRatio(attackerStatus.elemental, mSpellStatus.elemental);
	// HP�����Z
	mSpellStatus.hp -= damage;
	// �ϋv��0�ȉ��Ȃ�
	if (mSpellStatus.hp <= 0)
	{
		mSpellStatus.hp = 0;
		// �폜
		Kill();
	}
	// �_���[�W�\�L�𐶐�
	DamageText(damage, attackerStatus.elemental, attacker);
}

// �����ɂ��U���͔{�����v�Z����
float CSpellBase::ElementalPowerRatio(ESpellElementalType attacker, ESpellElementalType target)
{
	// �{��
	float ratio = 1.0f;

	switch (attacker)
	{
		// �U������
	case ESpellElementalType::eFire:
		// ��_
		// ���肪���Ȃ�
		if (target == ESpellElementalType::eWind)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// ���܂ЂƂ�
		// ���肪���Ȃ�
		if (target == ESpellElementalType::eWater)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;

		// �U������
	case ESpellElementalType::eWind:
		// ��_
		// ���肪�n�Ȃ�
		if (target == ESpellElementalType::eEarth)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// ���܂ЂƂ�
		// ���肪���Ȃ�
		if (target == ESpellElementalType::eFire)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;

		// �U�����n
	case ESpellElementalType::eEarth:
		// ��_
		// ���肪���Ȃ�
		if (target == ESpellElementalType::eThunder)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// ���܂ЂƂ�
		// ���肪���Ȃ�
		if (target == ESpellElementalType::eWind)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;
		
		// �U������
	case ESpellElementalType::eThunder:
		// ��_
		// ���肪���Ȃ�
		if (target == ESpellElementalType::eWater)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// ���܂ЂƂ�
		// ���肪�n�Ȃ�
		if (target == ESpellElementalType::eEarth)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;

		// �U������
	case ESpellElementalType::eWater:
		// ��_����
		// ���肪���Ȃ�
		if (target == ESpellElementalType::eFire)
		{
			ratio *= ELEMENTAL_POWER_RATIO;
		}

		// ���܂ЂƂ�
		// ���肪���Ȃ�
		if (target == ESpellElementalType::eThunder)
		{
			ratio /= ELEMENTAL_POWER_RATIO;
		}
		break;
	}

	return ratio;
}

// �����ɂ���ăJ���[��Ԃ�
CColor CSpellBase::ElementalColor(ESpellElementalType elemental)
{
	CColor color;
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		color = FIRE_COLOR;
		break;
	case ESpellElementalType::eWind:
		color = WIND_COLOR;
		break;
	case ESpellElementalType::eEarth:
		color = EARTH_COLOR;
		break;
	case ESpellElementalType::eThunder:
		color = THUNDER_COLOR;
		break;
	case ESpellElementalType::eWater:
		color = WATER_COLOR;
		break;
	case ESpellElementalType::eNeutral:
		color = NEUTRAL_COLOR;
		break;
	}
	return color;
}

// �_���[�W�\�L�𐶐�
void CSpellBase::DamageText(int damage, ESpellElementalType elemental,
	CSpellBase* attacker)
{	
	// �U�����̃_���[�W�\�L�𐶐�
	CDamageUI3D* text = new CDamageUI3D(Position());
	// �_���[�W�̐��l
	text->ChangeToStr("%d\n", damage);;
	// �������Ƃ̐F�𕶎��F�ɐݒ�
	text->SetFontColor(ElementalColor(elemental));
	CColor color;
	// �����傪�v���C���[�Ȃ�
	if (attacker->mpOwner == CPlayer::Instance())
	{
		color = PLAYER_COLOR;
	}
	// �G�Ȃ�
	else
	{
		color = ENEMY_COLOR;
	}
	// �A�E�g���C���̐F��ݒ�
	text->SetOutLineColor(color);
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
