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
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		mpModel = CResourceManager::Get<CModel>("FireBolt");
		break;
	case ESpellElementalType::eWind:
		mpModel = CResourceManager::Get<CModel>("WindBolt");
		break;
	case ESpellElementalType::eEarth:
		mpModel = CResourceManager::Get<CModel>("EarthBolt");
		break;
	case ESpellElementalType::eThunder:
		mpModel = CResourceManager::Get<CModel>("ThunderBolt");
		break;
	case ESpellElementalType::eWater:
		mpModel = CResourceManager::Get<CModel>("WaterBolt");
		break;
	case ESpellElementalType::eNeutral:
		mpModel = CResourceManager::Get<CModel>("NeutralBolt");
		break;
	}
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
	// �n�`�ƃv���C���[�ƓG�ƍU������Ɩh�䔻��Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::eGround,ELayer::eWall,ELayer::eObject,
		ELayer::ePlayer,ELayer::eEnemy,ELayer::eAttackCol,ELayer::eDefenseCol });
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
		// �ڕW�̕���������
		LookAt(mpTarget->Position());

		// �ړ������̍X�V�I��
		mStateStep++;
		break;
	}
	}
	// �ړ���ݒ�
	mMoveSpeed = mMoveDir * GetSpellStatus().speed * Times::DeltaTime();

	// ���N���X�̍X�V
	CSpellBase::UpdateShooting();
}