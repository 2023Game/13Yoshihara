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
	// �ҋ@���
	ChangeState(EState::eIdle);
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
	// �n�`�ƃv���C���[�ƓG�ƍU������Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eAttackCol });
}

// �ҋ@���̍X�V
void CBolt::UpdateIdle()
{
	// �ڕW�̕���������
	LookAt(mpTarget->Position());

	CSpellBase::UpdateIdle();
}

// ���˒��̍X�V
void CBolt::UpdateShooting()
{
	switch (mStateStep)
	{
		// �ڕW�ւ̕������ړ������ɐݒ�
	case 0:
	{
		mMoveDir = TargetDir();
		mMoveDir.Normalize();

		// �ړ������̍X�V�I��
		mStateStep++;
		break;
	}
	}
	// ���x
	float speed = GetSpellStatus().speed;
	// �ړ���ݒ�
	mMoveSpeed = mMoveDir * speed * Times::DeltaTime();

	// ����
	SetSpeed(speed + BOLT_ACCELERATE);

	// ���N���X�̍X�V
	CSpellBase::UpdateShooting();
}
