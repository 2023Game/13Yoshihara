#include "CBall.h"
#include "CColliderSphere.h"
#include "SpellDefine.h"

// �R���C�_�[�̔��a
#define RADIUS 1.0f

// �R���X�g���N�^
CBall::CBall(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBall, owner, target, BALL_DELETE_TIME)
{
	mpModel = CResourceManager::Get<CModel>("FireBall");
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
	// �n�`�ƃv���C���[�ƓG�ƍU������Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eAttackCol });
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

	// �ړ����x
	float speed = GetSpellStatus().speed;
	// �ړ����x��ݒ�
	mMoveSpeed = mMoveDir * speed * Times::DeltaTime();

	// ����
	SetSpeed(speed + BALL_ACCELERATE);

	// ���N���X�̍X�V
	CSpellBase::UpdateShooting();
}
