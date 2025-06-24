#include "CBall.h"
#include "CColliderSphere.h"
#include "SpellDefine.h"

// �R���C�_�[�̔��a
#define RADIUS 1.0f

// �R���X�g���N�^
CBall::CBall(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBall, owner, target, BALL_DELETE_TIME)
{
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		mpModel = CResourceManager::Get<CModel>("FireBall");
		break;
	case ESpellElementalType::eWind:
		mpModel = CResourceManager::Get<CModel>("WindBall");
		break;
	case ESpellElementalType::eEarth:
		mpModel = CResourceManager::Get<CModel>("EarthBall");
		break;
	case ESpellElementalType::eThunder:
		mpModel = CResourceManager::Get<CModel>("ThunderBall");
		break;
	case ESpellElementalType::eWater:
		mpModel = CResourceManager::Get<CModel>("WaterBall");
		break;
	case ESpellElementalType::eNeutral:
		mpModel = CResourceManager::Get<CModel>("NeutralBall");
		break;
	}
	// �R���C�_�[�𐶐�
	CreateCol();
	// �ҋ@���
	ChangeState(EState::eIdle);
}

// �f�X�g���N�^
CBall::~CBall()
{
}

// �R���C�_�[�𐶐�
void CBall::CreateCol()
{
	mpSpellCol = new CColliderSphere(
		this, ELayer::eAttackCol,
		RADIUS
	);
	// �n�`�ƃv���C���[�ƓG�ƍU������Ɩh�䔻��Ǝ����T�m�p�Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eAttackCol,ELayer::eDefenseCol,ELayer::eSpellSearch });
}

// ���˒��̍X�V
void CBall::UpdateShooting()
{
	switch (mStateStep)
	{
		// �ړ������̍X�V
	case 0:
	{
		mMoveSpeed = CVector::zero;
		// ����ւ̕���
		mMoveDir = TargetDir();
		// ����ւ̋���
		float dist = mMoveDir.Length();
		mMoveDir.Normalize();

		// �ǐՂ��鋗�����߂��Ȃ�����
		if (dist < BALL_CHASE_END_DIST)
		{
			// �c��̏��ł܂ł̎��Ԃ�
			// �ǐՏI����̏��Ŏ��Ԃ�蒷�����
			if (mDeleteTime - mElapsedTime > BALL_AFTER_CHASE_END_DELETE_TIME)
			{
				// �ǐՏI����̏��Ŏ��Ԃ�K�p
				mDeleteTime = BALL_AFTER_CHASE_END_DELETE_TIME;
				mElapsedTime = 0.0f;
			}
			// �ړ������̍X�V���I��
			mStateStep++;
		}
		break;
	}
	}

	// �ړ����x��ݒ�
	mMoveSpeed = mMoveDir * GetSpellStatus().speed * Times::DeltaTime();

	// ���N���X�̍X�V
	CSpellBase::UpdateShooting();
}