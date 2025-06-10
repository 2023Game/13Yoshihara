#include "CTeleport.h"
#include "CColliderSphere.h"
#include "CCharaBase.h"

// �R���C�_�[�̔��a
#define RADIUS 15.0f

// �R���X�g���N�^
CTeleport::CTeleport(ESpellElementalType elemental, CObjectBase* owner, CObjectBase* target)
	: CSpellBase(elemental, ESpellShapeType::eTeleport, owner, target, TELEPORT_DELETE_TIME)
{
	// �ŏ��X�P�[���ɐݒ�
	Scale(TELEPORT_SCALE_MIN);
	mpModel = CResourceManager::Get<CModel>("NeutralTeleport");
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
	mpSpellCol = new CColliderSphere(
		this, ELayer::eDefenseCol,
		RADIUS,
		true
	);
	// �U������Ƃ����Փ˔���
	mpSpellCol->SetCollisionLayers({ ELayer::eAttackCol });
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
		// �L�����N���X���擾
		CCharaBase* chara = dynamic_cast<CCharaBase*>(mpOwner);

		// �ړ�
		CVector moveSpeed = chara->GetMoveDir() * TELEPORT_MOVE_DIST;

		// �v���C���[�̍��W��ύX
		mpOwner->Position(mpOwner->Position() + moveSpeed);
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
