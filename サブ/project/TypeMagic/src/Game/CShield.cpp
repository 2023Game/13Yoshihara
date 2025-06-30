#include "CShield.h"
#include "CSpellCaster.h"
#include "CColliderRectangle.h"
#include "Maths.h"
#include <cmath>

// �R���C�_�[�̒��_
#define VERT_POS_1 CVector( 3.0f, 7.0f,-0.5f)
#define VERT_POS_2 CVector( 3.0f,-7.0f,-0.5f)
#define VERT_POS_3 CVector(-3.0f,-7.0f,-0.5f)
#define VERT_POS_4 CVector(-3.0f, 7.0f,-0.5f)

// �R���X�g���N�^
CShield::CShield(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eShield, owner, target, SHIELD_DELETE_TIME)
	, mCurrentAngle(0.0f)
	, mAnglePos(CVector::zero)
{
	// �ŏ��X�P�[���ɐݒ�
	Scale(SHIELD_SCALE_MIN);
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		mpModel = CResourceManager::Get<CModel>("FireShield");
		break;
	case ESpellElementalType::eWind:
		mpModel = CResourceManager::Get<CModel>("WindShield");
		break;
	case ESpellElementalType::eEarth:
		mpModel = CResourceManager::Get<CModel>("EarthShield");
		break;
	case ESpellElementalType::eThunder:
		mpModel = CResourceManager::Get<CModel>("ThunderShield");
		break;
	case ESpellElementalType::eWater:
		mpModel = CResourceManager::Get<CModel>("WaterShield");
		break;
	case ESpellElementalType::eNeutral:
		mpModel = CResourceManager::Get<CModel>("NeutralShield");
		break;
	}
	// �R���C�_�[�𐶐�
	CreateCol();
	// ���ˏ��
	ChangeState(EState::eShooting);
	// �e�ɂ��Ă���
	SetParent(mpOwner);
	// �V�[���h�𒣂��Ă���
	CSpellCaster* caster = dynamic_cast<CSpellCaster*>(mpOwner);
	caster->SetIsShield(true);
}

// �f�X�g���N�^
CShield::~CShield()
{
}

// �X�V
void CShield::Update()
{
	// �V�[���h�𒣂��Ă���
	CSpellCaster* caster = dynamic_cast<CSpellCaster*>(mpOwner);
	caster->SetIsShield(true);

	// ������̕���������
	LookAt(mpOwner->Position());

	// ���N���X�X�V
	CSpellBase::Update();
}

// �R���C�_�[�𐶐�
void CShield::CreateCol()
{
	mpSpellCol = new CColliderRectangle(
		this, ELayer::eDefenseCol,
		VERT_POS_1,
		VERT_POS_2,
		VERT_POS_3,
		VERT_POS_4,
		true
	);
	// �U������Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::eAttackCol });
}

// ���˒��̍X�V
void CShield::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// �傫���Ȃ��Ă���
	case 0:
		// �g��
		Scale(Scale() + SHIELD_SCALE_UP_NUM);

		// �ő�X�P�[���ȏ�ɂȂ�����
		if (Scale().X() >= SHIELD_SCALE_MAX.X())
		{
			mElapsedTime = 0.0f;
			// �ő�ɐݒ�
			Scale(SHIELD_SCALE_MAX);
			// �g��I��
			mStateStep++;
		}
		break;
		// �폜���Ԃ��o�߂����玟��
	case 1:
		if (mElapsedTime > mDeleteTime)
		{
			mStateStep++;
		}
		break;
		// �ŏ��X�P�[���ɂȂ�����폜
	case 2:
		// �k��
		Scale(Scale() - SHIELD_SCALE_UP_NUM);
		// �ŏ��X�P�[���ȉ��ɂȂ�����
		if (Scale().X() <= SHIELD_SCALE_MIN.X())
		{
			// �폜
			Kill();
		}
		break;
	}
	// �p�x�𑝉�
	mCurrentAngle += SHIELD_ROT_SPEED * Times::DeltaTime();
	// �ۂ߂�
	if (mCurrentAngle >= 360.0f) mCurrentAngle -= 360.0f;

	// ��]�p�x
	float rad = mCurrentAngle * M_PI / 180.0f;

	// ��]��̃I�t�Z�b�g���W
	CVector offsetPos =
		CVector(
			cos(rad) * SHIELD_ROT_RADIUS,
			0.0f,
			sin(rad) * SHIELD_ROT_RADIUS);

	// ���W�X�V
	Position(mpOwner->Position() + offsetPos);
}

// �폜
void CShield::Kill()
{
	// �����Ă��Ȃ�
	CSpellCaster* caster = dynamic_cast<CSpellCaster*>(mpOwner);
	caster->SetIsShield(false);
	// ���N���X�̍폜����
	CTask::Kill();
}
