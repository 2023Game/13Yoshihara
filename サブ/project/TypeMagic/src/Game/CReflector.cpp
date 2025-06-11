#include "CReflector.h"
#include "CColliderRectangle.h"
#include "Maths.h"
#include <cmath>
#include "CSpellCaster.h"

// �R���C�_�[�̒��_
#define VERT_POS_1 CVector( 10.0f, 10.0f,-0.5f)
#define VERT_POS_2 CVector( 10.0f,-10.0f,-0.5f)
#define VERT_POS_3 CVector(-10.0f,-10.0f,-0.5f)
#define VERT_POS_4 CVector(-10.0f, 10.0f,-0.5f)

// �R���X�g���N�^
CReflector::CReflector(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eReflector, owner, target, REFLECTOR_DELETE_TIME)
{
	// �ŏ��X�P�[���ɐݒ�
	Scale(REFLECTOR_SCALE_MIN);
	mpModel = CResourceManager::Get<CModel>("NeutralReflector");
	// �R���C�_�[�𐶐�
	CreateCol();
	// ���ˏ��
	ChangeState(EState::eShooting);
	// �e�ɂ��Ă���
	SetParent(mpOwner);
}

// �f�X�g���N�^
CReflector::~CReflector()
{
}

// �X�V
void CReflector::Update()
{
	// ������̕���������
	LookAt(mpOwner->Position());

	// ���N���X�X�V
	CSpellBase::Update();
}

// �R���C�_�[�𐶐�
void CReflector::CreateCol()
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
void CReflector::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// �傫���Ȃ��Ă���
	case 0:
		// �g��
		Scale(Scale() + REFLECTOR_SCALE_UP_NUM);

		// �ő�X�P�[���ȏ�ɂȂ�����
		if (Scale().X() >= REFLECTOR_SCALE_MAX.X())
		{
			mElapsedTime = 0.0f;
			// �ő�ɐݒ�
			Scale(REFLECTOR_SCALE_MAX);
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
		Scale(Scale() - REFLECTOR_SCALE_UP_NUM);
		// �ŏ��X�P�[���ȉ��ɂȂ�����
		if (Scale().X() <= REFLECTOR_SCALE_MIN.X())
		{
			// �폜
			Kill();
		}
		break;
	}
}