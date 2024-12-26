#include "CVehicleBase.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CVehicleManager.h"
 
#define FRONT_HEIGHT	13.0f	// �O������̍���
#define FRONT_WIDTH		30.0f	// �O������̕�
#define FRONT_RADIUS	12.0f	// �O������̔��a
#define TURN_SPEED		CVector(0.0f,0.5f,0.0f)	// �ԗ��̕����]�����x
#define TURN_MAX		CVector(0.0f,22.5f,0.0f)// �ԗ��̕����]���̍ő�l

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
}

// �`��
void CVehicleBase::Render()
{
	mpModel->Render(Matrix());
}

// �ړ������ǂ���
bool CVehicleBase::IsMove() const
{
	// �ړ����x��2��̒�����0���傫���Ȃ�ړ���
	if (mMoveSpeed.LengthSqr() > 0.0f) return true;
	// �S��0�ȉ��Ȃ�ړ����Ă��Ȃ�
	return false;
}

// �Ԑ���ύX����
void CVehicleBase::ChangeRoad(float moveSpeed, bool& isEnd)
{
	isEnd = false;
	// ���ʂֈړ�����
	mMoveSpeed = VectorZ() * moveSpeed;

	// ���̓����m��X����
	float xDistLeft = std::abs(CAR_LEFT_POS1.X() - CAR_LEFT_POS2.X());
	// �E�̓����m��X����
	float xDistRight = std::abs(CAR_RIGHT_POS1.X() - CAR_RIGHT_POS2.X());

	// ���̓����m�̒��ԓ_��X���W
	float leftMid = CAR_LEFT_POS1.X() + xDistLeft;
	// �E�̓����m�̒��ԓ_��X���W
	float rightMid = CAR_RIGHT_POS1.X() - xDistLeft;

	// �����铹�ɂ���ď���
	switch (GetRoadType())
	{
	// �������Ԗڂ̓�
	case ERoadType::eLeft1:
		// �܂����̓����m�̒��ԓ_�𒴂��Ă��Ȃ�����
		// ��]���A���̓��̏����l + �����]���̍ő�l��菬�������
		if (Position().X() <= leftMid &&
			EulerAngles().Y() <= CAR_LEFT_ROTATION.Y() + TURN_MAX.Y())
		{
			// �E������
			Rotation(EulerAngles() + TURN_SPEED);
		}
		// ���ԓ_�𒴂��Ă��邩��
		// ���̕����𒴂��Ă��Ȃ�
		else if (Position().X() >= leftMid &&
			EulerAngles().Y() >= CAR_LEFT_ROTATION.Y())
		{
			// ���̕����ɖ߂��Ă���
			Rotation(EulerAngles() - TURN_SPEED);
		}

		// X���W���������Ԗڂ̓��̍��W�ȏ�ɂȂ��
		if (Position().X() >= CAR_LEFT_POS2.X())
		{
			// �����铹�̏�Ԃ��������Ԗڂ̓��ɕύX
			ChangeRoadType(ERoadType::eLeft2);
			Rotation(CAR_LEFT_ROTATION);
			isEnd = true;
		}
		break;
	// �������Ԗڂ̓�
	case ERoadType::eLeft2:
		// �܂����̓����m�̒��ԓ_�𒴂��Ă��Ȃ�����
		// ��]���A���̓��̏����l - �����]���̍ő�l���傫�����
		if (Position().X() >= leftMid &&
			EulerAngles().Y() >= CAR_LEFT_ROTATION.Y() - TURN_MAX.Y())
		{
			// ��������
			Rotation(EulerAngles() - TURN_SPEED);
		}
		// ���ԓ_�𒴂��Ă��邩��
		// ���̕����𒴂��Ă��Ȃ�
		else if (Position().X() <= leftMid &&
			EulerAngles().Y() <= CAR_LEFT_ROTATION.Y())
		{
			// �E�̕����ɖ߂��Ă���
			Rotation(EulerAngles() + TURN_SPEED);
		}

		// X���W���������Ԗڂ̓��̍��W�ȉ��ɂȂ��
		if (Position().X() <= CAR_LEFT_POS1.X())
		{
			// �����铹�̏�Ԃ��������Ԗڂ̓��ɕύX
			ChangeRoadType(ERoadType::eLeft1);
			Rotation(CAR_LEFT_ROTATION);
			isEnd = true;
		}
		break;
	// �E�����Ԗڂ̓�
	case ERoadType::eRight1:
		// �܂��E�̓����m�̒��ԓ_�𒴂��Ă��Ȃ�����
		// ��]���A�E�̓��̏����l + �����]���̍ő�l��菬�������
		if (Position().X() >= rightMid &&
			EulerAngles().Y() <= CAR_RIGHT_ROTATION.Y() + TURN_MAX.Y())
		{
			// �E������
			Rotation(EulerAngles() + TURN_SPEED);
		}
		// ���ԓ_�𒴂��Ă��邩��
		// ���̕����𒴂��Ă��Ȃ�
		else if (Position().X() <= rightMid &&
			EulerAngles().Y() <= CAR_RIGHT_ROTATION.Y())
		{
			// ���̕����ɖ߂��Ă���
			Rotation(EulerAngles() - TURN_SPEED);
		}

		// X���W���E�����Ԗڂ̓��̍��W�ȉ��ɂȂ��
		if (Position().X() <= CAR_RIGHT_POS2.X())
		{
			// �����铹�̏�Ԃ��E�����Ԗڂ̓��ɕύX
			ChangeRoadType(ERoadType::eRight2);
			Rotation(CAR_RIGHT_ROTATION);
			isEnd = true;
		}
		break;
	// �E�����Ԗڂ̓�
	case ERoadType::eRight2:
		// �܂��E�̓����m�̒��ԓ_�𒴂��Ă��Ȃ�����
		// ��]���A�E�̓��̏����l - �����]���̍ő�l���傫�����
		if (Position().X() <= rightMid &&
			EulerAngles().Y() >= CAR_RIGHT_ROTATION.Y() - TURN_MAX.Y())
		{
			// ��������
			Rotation(EulerAngles() - TURN_SPEED);
		}
		// ���ԓ_�𒴂��Ă��邩��
		// ���̕����𒴂��Ă��Ȃ�
		else if (Position().X() >= rightMid &&
			EulerAngles().Y() >= CAR_RIGHT_ROTATION.Y())
		{
			// �E�̕����ɖ߂��Ă���
			Rotation(EulerAngles() + TURN_SPEED);
		}

		// �܂����ԓ_�𒴂��Ă��Ȃ�����
		// ��]���A�E�̓��̏����l - �����]���̍ő�l��菬�����Ȃ����
		else if (!(EulerAngles().Y() <= CAR_RIGHT_ROTATION.Y() - TURN_MAX.Y()))
		{
			// ��������
			Rotation(EulerAngles() - TURN_SPEED);
		}

		// X���W���E�����Ԗڂ̓��̍��W�ȏ�ɂȂ��
		if (Position().X() >= CAR_RIGHT_POS1.X())
		{
			// �����铹�̏�Ԃ��E�����Ԗڂ̓��ɕύX
			ChangeRoadType(ERoadType::eRight1);
			Rotation(CAR_RIGHT_ROTATION);
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
