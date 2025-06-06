#include "CBolt.h"
#include "CColliderCapsule.h"

// �R���C�_�[�̕�
#define WIDTH 8.0f
// �R���C�_�[�̔��a
#define RADIUS 0.5f

// �R���X�g���N�^
CBolt::CBolt(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBolt, owner, target, BOLT_DELETE_TIME)
{
	mpModel = CResourceManager::Get<CModel>("FireBolt");
	// �R���C�_�[�𐶐�
	CreateCol();
}

// �f�X�g���N�^
CBolt::~CBolt()
{
}

// �R���C�_�[�𐶐�
void CBolt::CreateCol()
{
	mpSpellCol = new CColliderCapsule(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, WIDTH),
		RADIUS
	);
	// �n�`�ƃv���C���[�ƓG�Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy });
}

// �ҋ@���̍X�V
void CBolt::UpdateIdle()
{
	CVector targetPos = mpTarget->Position();
	targetPos.Y(targetPos.Y() + TARGET_HEIGHT);
	// �ڕW�̕���������
	LookAt(targetPos);

	CSpellBase::UpdateIdle();
}

// ���˒��̍X�V
void CBolt::UpdateShooting()
{
	switch (mStateStep)
	{
		// �ڕW���������߂�
	case 0:
	{
		// �_�������𑫂�
		CVector targetPos = mpTarget->Position();
		targetPos.Y(targetPos.Y() + TARGET_HEIGHT);
		// �ڕW�ւ̕���
		mMoveDir = targetPos - Position();
		mMoveDir.Normalize();

		// ����
		mStateStep++;
		break;
	}
		
		// �ڕW�����ֈړ�
	case 1:
		float speed = GetSpellStatus().speed;

		// �ڕW�����Ɍ������Ĉړ�
		mMoveSpeed = mMoveDir * speed * Times::DeltaTime();

		// ����
		SetSpeed(speed + BOLT_ACCELERATE);
		break;
	}

	// ���N���X�̍X�V
	CSpellBase::UpdateShooting();
}
