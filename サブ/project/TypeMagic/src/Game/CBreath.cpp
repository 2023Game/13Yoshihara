#include "CBreath.h"
#include "CColliderSphere.h"
#include "Maths.h"

// �R���C�_�[�̔��a
#define RADIUS 3.0f

// �R���X�g���N�^
CBreath::CBreath(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eBreath, owner, target, BREATH_DELETE_TIME)
{
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		mpModel = CResourceManager::Get<CModel>("FireBreath");
		break;
	case ESpellElementalType::eWind:
		mpModel = CResourceManager::Get<CModel>("WindBreath");
		break;
	case ESpellElementalType::eEarth:
		mpModel = CResourceManager::Get<CModel>("EarthBreath");
		break;
	case ESpellElementalType::eThunder:
		mpModel = CResourceManager::Get<CModel>("ThunderBreath");
		break;
	case ESpellElementalType::eWater:
		mpModel = CResourceManager::Get<CModel>("WaterBreath");
		break;
	case ESpellElementalType::eNeutral:
		mpModel = CResourceManager::Get<CModel>("NeutralBreath");
		break;
	}
	// �R���C�_�[�𐶐�
	CreateCol();
	// �ŏ����甭�ˏ��
	ChangeState(EState::eShooting);
}

// �f�X�g���N�^
CBreath::~CBreath()
{
}

// �R���C�_�[�𐶐�
void CBreath::CreateCol()
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
void CBreath::UpdateShooting()
{
	switch (mStateStep)
	{
		// �ړ����������߂�
	case 0:
		// �ڕW�ւ̕���
		mMoveDir = TargetDir();
		mMoveDir.Normalize();
		// �ړ������������_���łԂ炷
		mMoveDir.X(mMoveDir.X() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
		mMoveDir.Y(mMoveDir.Y() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
		mMoveDir.Z(mMoveDir.Z() + Math::Rand(-BREATH_DIR_RAND, BREATH_DIR_RAND));
		mMoveDir.Normalize();
		// �ړ������̍X�V���I��
		mStateStep++;
		break;
	}

	// �傫������
	Scale(Scale() + BREATH_SCALE_UP_NUM);

	// �ړ���ݒ�
	mMoveSpeed = mMoveDir * GetSpellStatus().speed * Times::DeltaTime();

	// ���N���X�̍X�V
	CSpellBase::UpdateShooting();
}