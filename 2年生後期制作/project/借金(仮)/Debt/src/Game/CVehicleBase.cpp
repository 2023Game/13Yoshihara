#include "CVehicleBase.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CVehicleManager.h"
 
#define FRONT_HEIGHT	13.0f	// �O������̍���
#define FRONT_WIDTH		30.0f	// �O������̕�
#define FRONT_RADIUS	12.0f	// �O������̔��a

// �R���X�g���N�^
CVehicleBase::CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CCharaBase(ETag::eVehicle, ETaskPriority::eVehicle)
	, mpModel(model)
	, mpBodyCol(nullptr)
	, mpFrontCol(nullptr)
	, mpSideCol(nullptr)
	, mRoadType(road)
{
	// �ԗ��ƏՓ˔��肷��O������R���C�_�\
	mpFrontCol = new CColliderCapsule
	(
		this, ELayer::eVehicle,
		CVector(0.0f, FRONT_HEIGHT, FRONT_WIDTH),
		CVector(0.0f, FRONT_HEIGHT, -FRONT_WIDTH),
		FRONT_RADIUS
	);
	mpFrontCol->Position(0.0f, 0.0f, FRONT_WIDTH * 2);
	mpFrontCol->SetCollisionTags({ETag::eVehicle });
	mpFrontCol->SetCollisionLayers({ELayer::eVehicle,});

	Position(pos);
	Rotation(rotation);
}

// �f�X�g���N�^
CVehicleBase::~CVehicleBase()
{
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpFrontCol);
	SAFE_DELETE(mpSideCol);
}

// �X�V
void CVehicleBase::Update()
{
	CVector moveSpeed = mMoveSpeed;

	Position(Position() + moveSpeed);
}

// �Փˏ���
void CVehicleBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̃R���C�_�\
	if (self == mpBodyCol)
	{
		// �Փ˂������肪�v���C���[�̏ꍇ
		if (other->Layer() == ELayer::ePlayer)
		{
			// �����������ɉ����߂�
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
}

// �`��
void CVehicleBase::Render()
{
	mpModel->Render(Matrix());
}

// �ړ������ǂ���
bool CVehicleBase::IsMove() const
{
	// �e���ւ̈ړ����x�̂����ꂩ��0���傫���Ȃ�ړ���
	if (mMoveSpeed.X() > 0.0f) return true;
	if (mMoveSpeed.Y() > 0.0f) return true;
	if (mMoveSpeed.Z() > 0.0f) return true;
	// �S��0�ȉ��Ȃ�ړ����Ă��Ȃ�
	return false;
}

// �Ԑ���ύX����
void CVehicleBase::ChangeRoad(float moveSpeed, bool& isEnd)
{
	isEnd = false;
	// �����铹�ɂ���ď���
	switch (GetRoadType())
	{
	// �������Ԗڂ̓�
	case ERoadType::eLeft1:
		// �������猩�ĉE�ֈړ�
		mMoveSpeed = VectorX() * moveSpeed;
		// X���W���������Ԗڂ̓��̍��W�ȏ�ɂȂ��
		if (Position().X() >= CAR_LEFT_POS2.X())
		{
			// �����铹�̏�Ԃ��������Ԗڂ̓��ɕύX
			ChangeRoadType(ERoadType::eLeft2);
			isEnd = true;
		}
		break;
	// �������Ԗڂ̓�
	case ERoadType::eLeft2:
		// �������猩�č��ֈړ�
		mMoveSpeed = -VectorX() * moveSpeed;
		// X���W���������Ԗڂ̓��̍��W�ȉ��ɂȂ��
		if (Position().X() <= CAR_LEFT_POS1.X())
		{
			// �����铹�̏�Ԃ��������Ԗڂ̓��ɕύX
			ChangeRoadType(ERoadType::eLeft1);
			isEnd = true;
		}
		break;
	// �E�����Ԗڂ̓�
	case ERoadType::eRight1:
		// �������猩�č��ֈړ�
		mMoveSpeed = VectorX() * moveSpeed;
		// X���W���E�����Ԗڂ̓��̍��W�ȉ��ɂȂ��
		if (Position().X() <= CAR_RIGHT_POS2.X())
		{
			// �����铹�̏�Ԃ��E�����Ԗڂ̓��ɕύX
			ChangeRoadType(ERoadType::eRight2);
			isEnd = true;
		}
		break;
	// �E�����Ԗڂ̓�
	case ERoadType::eRight2:
		// �������猩�ĉE�ֈړ�
		mMoveSpeed = -VectorX() * moveSpeed;
		// X���W���E�����Ԗڂ̓��̍��W�ȏ�ɂȂ��
		if (Position().X() >= CAR_RIGHT_POS1.X())
		{
			// �����铹�̏�Ԃ��E�����Ԗڂ̓��ɕύX
			ChangeRoadType(ERoadType::eRight1);
			isEnd = true;
		}
		break;
	}
}

// �ǂ̓��ɂ����Ԃ���ύX����
void CVehicleBase::ChangeRoadType(ERoadType roadType)
{
	// �����Ȃ珈�����Ȃ�
	if (roadType == mRoadType) return;

	mRoadType = roadType;
}

// ���ǂ̓��ɂ��邩�擾����
CVehicleBase::ERoadType CVehicleBase::GetRoadType() const
{
	return mRoadType;
}
