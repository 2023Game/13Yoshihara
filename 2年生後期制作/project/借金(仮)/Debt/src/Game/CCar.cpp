#include "CCar.h"
#include "CColliderCapsule.h"
#include "CModel.h"

#define CAR_HEIGHT		9.0f	// �Ԃ̍���
#define CAR_WIDTH		25.0f	// �Ԃ̕�
#define CAR_RADIUS	10.0f	// �Ԃ̔��a

CCar::CCar(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CVehicleBase(model, pos, rotation, road)
	, mState(EState::eMove)
{
	// �v���C���[�A�G�A�����ꏊ�A�ԗ��A�n�`
	// �ƏՓ˔��肷��{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eVehicle,
		CVector(0.0f, CAR_HEIGHT, CAR_WIDTH - CAR_RADIUS),
		CVector(0.0f, CAR_HEIGHT, -CAR_WIDTH + CAR_RADIUS),
		CAR_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,ETag::eSpawnZone,
		ETag::eVehicle,ETag::eField, });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eSpawnZone,ELayer::eVehicle,
		ELayer::eField,ELayer::eWall,ELayer::eObject });

	// �ŏ��͕`��A�X�V���Ȃ�
	SetEnable(false);
	SetShow(false);
}

CCar::~CCar()
{

}

void CCar::Update()
{
	switch (mState)
	{
	// �ړ�
	case EState::eMove:		UpdateMove();	break;
	// ��~
	case EState::eStop:		UpdateStop();	break;
	// ��ꂽ
	case EState::eBroken:	UpdateBroken();	break;
	// �Ԑ��ύX
	case EState::eChangeRoad: UpdateChangeRoad(); break;
	}

	CVehicleBase::Update();

#if _DEBUG
	CDebugPrint::Print("CarState:%s\n", GetStateStr(mState).c_str());
#endif
}

// �Փˏ���
void CCar::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CVehicleBase::Collision(self, other, hit);

	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �Փ˂������肪�v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// ��ꂽ��ԂɕύX
			ChangeState(EState::eBroken);
		}
	}
	// �O������R���C�_�\
	if (self == mpFrontCol)
	{
		// �Փ˂������肪�ԗ��̏ꍇ
		if (other->Layer() == ELayer::eVehicle)
		{
			// �ԗ��N���X���擾
			CVehicleBase* vehicle = dynamic_cast<CVehicleBase*>(other->Owner());
			// ���肪�����Ă��Ȃ����
			if (!vehicle->IsMove())
			{
				// �Ԑ��ύX��Ԃ�
				ChangeState(EState::eChangeRoad);
			}
		}
	}
}

// �ړ�����
void CCar::UpdateMove()
{
	// ���ʂֈړ�
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed();
}

// ��~����
void CCar::UpdateStop()
{
	// �ړ����x���[���ɂ���
	mMoveSpeed = CVector::zero;
}

// ��ꂽ����
// �ړ����~���ď��Ŏ��Ԃ��o������\��������
void CCar::UpdateBroken()
{
	// �ړ����x���[���ɂ���
	mMoveSpeed = CVector::zero;

	// ���ł���܂ł̎��Ԃ��J�E���g�_�E��
	CountDeleteTime();

	// ���ł܂ł̎��Ԃ��o�߂�����
	if (IsElapsedDeleteTime())
	{
		// ���ł܂ł̎��Ԃ������l�ɖ߂�
		SetDeleteTime();
		// ��Ԃ��ړ��ɖ߂��Ă���
		ChangeState(EState::eMove);

		// ��\��
		SetEnable(false);
		SetShow(false);
	}
}

// �Ԑ��ύX����
void CCar::UpdateChangeRoad()
{
	bool isEnd = false;
	// �Ԑ��ύX�ړ�
	ChangeRoad(GetBaseMoveSpeed(), isEnd);

	// true�Ȃ�΁A�Ԑ��ύX���I�����
	if (isEnd)
	{
		// �ړ���Ԃɖ߂�
		ChangeState(EState::eMove);
	}
}

// ��Ԑ؂�ւ�
void CCar::ChangeState(EState state)
{
	// �����Ȃ珈�����Ȃ�
	if (state == mState) return;

	mState = state;
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CCar::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:		return "�ړ���";
	case EState::eStop:		return "��~��";
	case EState::eBroken:	return "���Ă���";
	default:				return "";
	}
}
#endif
