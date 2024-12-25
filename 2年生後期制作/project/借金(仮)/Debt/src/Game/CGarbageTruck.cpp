#include "CGarbageTruck.h"
#include "CColliderCapsule.h"
#include "CModel.h"

#define TRUCK_HEIGHT	13.0f	// �g���b�N�̍���
#define TRUCK_WIDTH		27.0f	// �g���b�N�̕�
#define TRUCK_RADIUS	12.0f	// �g���b�N�̔��a

// �R���X�g���N�^
CGarbageTruck::CGarbageTruck(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CVehicleBase(model, pos, rotation, road)
	, mState(EState::eMove)
{
	// �v���C���[�A�G�A�����ꏊ�A�ԗ��A�n�`
	// �ƏՓ˔��肷��{�̃R���C�_�\
	mpBodyCol = new CColliderCapsule
	(
		this,ELayer::eVehicle,
		CVector(0.0f,TRUCK_HEIGHT,TRUCK_WIDTH - TRUCK_RADIUS),
		CVector(0.0f,TRUCK_HEIGHT,-TRUCK_WIDTH + TRUCK_RADIUS),
		TRUCK_RADIUS
	);
	mpBodyCol->Position(0.0f, 0.0f, -2.0f);
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,ETag::eSpawnZone,
		ETag::eVehicle,ETag::eField, });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,
		ELayer::eSpawnZone,ELayer::eVehicle,
		ELayer::eField,ELayer::eWall,ELayer::eObject });

	// �ŏ��͕`��A�X�V���Ȃ�
	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CGarbageTruck::~CGarbageTruck()
{
}

// �X�V
void CGarbageTruck::Update()
{
	switch (mState)
	{
	// �ړ�
	case EState::eMove:		UpdateMove();		break;
	// ��~
	case EState::eStop:		UpdateStop();		break;
	// ��ꂽ
	case EState::eBroken:	UpdateBroken();		break;
	// ���
	case EState::eCollect:	UpdateCollect();	break;
	}

	CVehicleBase::Update();

#if _DEBUG
	CDebugPrint::Print("TruckState:%s\n", GetStateStr(mState).c_str());
#endif
}

// �Փˏ���
void CGarbageTruck::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CVehicleBase::Collision(self, other, hit);

	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �Փ˂������肪�v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// ��Ԃ��~�ɕύX
			ChangeState(EState::eStop);
		}
	}
}

// �ړ�����
void CGarbageTruck::UpdateMove()
{
	// ���ʂֈړ�
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed();
}

// ��~����
void CGarbageTruck::UpdateStop()
{
	// �ړ����x���[���ɂ���
	mMoveSpeed = CVector::zero;
}

// ��ꂽ����
// �ړ����~���ď��Ŏ��Ԃ��o������\��������
void CGarbageTruck::UpdateBroken()
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

// �������
void CGarbageTruck::UpdateCollect()
{
}

// ��Ԑ؂�ւ�
void CGarbageTruck::ChangeState(EState state)
{
	// �����Ȃ珈�����Ȃ�
	if (state == mState) return;

	mState = state;
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CGarbageTruck::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eMove:		return "�ړ���";
	case EState::eStop:		return "��~��";
	case EState::eBroken:	return "���Ă���";
	case EState::eCollect:	return "�����";
	default:				return "";
	}
}
#endif