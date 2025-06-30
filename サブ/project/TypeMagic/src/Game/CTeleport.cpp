#include "CTeleport.h"
#include "CColliderSphere.h"
#include "CCharaBase.h"
#include "CFieldBase.h"

// �R���C�_�[�̔��a
#define RADIUS 15.0f

// �R���X�g���N�^
CTeleport::CTeleport(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eTeleport, owner, target, TELEPORT_DELETE_TIME)
{
	// �ŏ��X�P�[���ɐݒ�
	Scale(TELEPORT_SCALE_MIN);
	switch (elemental)
	{
	case ESpellElementalType::eFire:
		mpModel = CResourceManager::Get<CModel>("FireTeleport");
		break;
	case ESpellElementalType::eWind:
		mpModel = CResourceManager::Get<CModel>("WindTeleport");
		break;
	case ESpellElementalType::eEarth:
		mpModel = CResourceManager::Get<CModel>("EarthTeleport");
		break;
	case ESpellElementalType::eThunder:
		mpModel = CResourceManager::Get<CModel>("ThunderTeleport");
		break;
	case ESpellElementalType::eWater:
		mpModel = CResourceManager::Get<CModel>("WaterTeleport");
		break;
	case ESpellElementalType::eNeutral:
		mpModel = CResourceManager::Get<CModel>("NeutralTeleport");
		break;
	}
	// �R���C�_�[�𐶐�
	CreateCol();
	// �ҋ@���
	ChangeState(EState::eIdle);
	// �R���C�_�[��L��
	mpSpellCol->SetEnable(true);
}

// �f�X�g���N�^
CTeleport::~CTeleport()
{
}

// �R���C�_�[�𐶐�
void CTeleport::CreateCol()
{
	// �T�|�[�g���������U�����������
	mpSpellCol = new CColliderSphere(
		this, ELayer::eAttackCol,
		RADIUS,
		true
	);
	// �v���C���[�ƓG�ƍU������Ɩh�䔻��Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eAttackCol,ELayer::eDefenseCol });
}

// �ҋ@���̍X�V
void CTeleport::UpdateIdle()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// �傫���Ȃ��Ă���
	case 0:
		// �g��
		Scale(Scale() + TELEPORT_SCALE_UP_NUM);

		// �ő�X�P�[���ȏ�ɂȂ�����
		if (Scale().X() >= TELEPORT_SCALE_MAX.X())
		{
			mElapsedTime = 0.0f;
			// �ő�ɐݒ�
			Scale(TELEPORT_SCALE_MAX);
			// ������̍X�V�ƕ`��𖳌�
			mpOwner->SetEnable(false);
			mpOwner->SetShow(false);
			// ����
			mStateStep++;
		}
		break;

		// �������Ȃ��Ă���
	case 1:
		// �k��
		Scale(Scale() - TELEPORT_SCALE_UP_NUM);

		// �ŏ��X�P�[���ȉ��ɂȂ�����
		if (Scale().X() <= TELEPORT_SCALE_MIN.X())
		{
			mElapsedTime = 0.0f;
			// �ŏ��ɐݒ�
			Scale(TELEPORT_SCALE_MIN);
			// ����
			mStateStep++;
		}
		break;

	case 2:
		// �ҋ@���Ԃ��o�߂�����
		if (mElapsedTime > TELEPORT_IDLE_TIME)
		{
			// ���ˏ�Ԃ�
			ChangeState(EState::eShooting);
			// �e�ɂ��Ă���
			SetParent(mpOwner);
			return;
		}
		break;
	}
}

// ���˒��̍X�V
void CTeleport::UpdateShooting()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// �ړ�
	case 0:
	{
		// �q�b�g�����I�u�W�F�N�g�̃��X�g�����Z�b�g
		AttackStart();
		// �L�����N���X���擾
		CCharaBase* chara = dynamic_cast<CCharaBase*>(mpOwner);

		// �ړ�
		CVector moveSpeed = chara->GetMoveDir() * TELEPORT_MOVE_DIST;

		// �V�������W
		CVector newPos = mpOwner->Position() + moveSpeed;

		CHitInfo hit;
		// TODO�F�t�B�[���h�𐶐�����Ȃ�R�����g����
		// �t�B�[���h�ƏՓ˂��Ă���Ȃ�
		//if (CFieldBase::Instance()->CollisionRay(mpOwner->Position(), newPos, &hit))
		//{
		//	// �Փˈʒu��V�������W�ɂ���
		//	newPos = hit.cross;
		//}

		// ������̍��W��ύX
		mpOwner->Position(newPos);
		mStateStep++;
		break;
	}

		// �傫���Ȃ��Ă���
	case 1:
		// �g��
		Scale(Scale() + TELEPORT_SCALE_UP_NUM);

		// �ő�X�P�[���ȏ�ɂȂ�����
		if (Scale().X() >= TELEPORT_SCALE_MAX.X())
		{
			mElapsedTime = 0.0f;
			// �ő�ɐݒ�
			Scale(TELEPORT_SCALE_MAX);
			// ������̍X�V�ƕ`���L��
			mpOwner->SetEnable(true);
			mpOwner->SetShow(true);
			// ����
			mStateStep++;
		}
		break;

		// �������Ȃ��Ă���
	case 2:
		// �k��
		Scale(Scale() - TELEPORT_SCALE_UP_NUM);

		// �ŏ��X�P�[���ȉ��ɂȂ�����
		if (Scale().X() <= TELEPORT_SCALE_MIN.X())
		{
			// �폜
			Kill();
		}

		break;
	}
}