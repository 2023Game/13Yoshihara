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
}

// �f�X�g���N�^
CBall::~CBall()
{
}

// �X�V����
void CBall::Update()
{
	// ���N���X�̍X�V����
	CSpellBase::Update();
}

// �R���C�_�[�𐶐�
void CBall::CreateCol()
{
	mpSpellCol = new CColliderSphere(
		this, ELayer::eAttackCol,
		RADIUS
	);
	// �n�`�ƃv���C���[�ƓG�Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy });
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
		// ����̈ʒu
		CVector opponentPos = mpTarget->Position();
		opponentPos.Y(opponentPos.Y() + TARGET_HEIGHT);
		// ����ւ̕���
		mMoveDir = opponentPos - Position();
		// ����ւ̋���
		float dist = mMoveDir.Length();
		mMoveDir.Normalize();

		// �ǐՂ��鋗�����߂��Ȃ�����
		if (dist < BALL_CHASE_END_DIST)
		{
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
