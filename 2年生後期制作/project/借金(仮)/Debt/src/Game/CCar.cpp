#include "CCar.h"
#include "CColliderCapsule.h"
#include "CModel.h"

#define CAR_HEIGHT		9.0f	// �Ԃ̍���
#define CAR_WIDTH		25.0f	// �Ԃ̕�
#define CAR_RADIUS	10.0f	// �Ԃ̔��a

CCar::CCar(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CVehicleBase(model, pos, rotation, road)
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
	case EState::eMove:			UpdateMove();		break;
	// ��~
	case EState::eStop:			UpdateStop();		break;
	// ��ꂽ
	case EState::eBroken:		UpdateBroken();		break;
	// �Ԑ��ύX
	case EState::eChangeRoad:	UpdateChangeRoad(); break;
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
}
