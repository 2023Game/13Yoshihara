#include "CVehicleBase.h"
#include "CModel.h"
#include "CColliderCapsule.h"
#include "CVehicleManager.h"
#include "CNavManager.h"
#include "CNavNode.h"
 
#define FRONT_HEIGHT	13.0f	// �O������̍���
#define FRONT_WIDTH		40.0f	// �O������̕�
#define FRONT_RADIUS	12.0f	// �O������̔��a
#define TURN_SPEED		CVector(0.0f,0.5f,0.0f)	// �ԗ��̕����]�����x
#define TURN_MAX		CVector(0.0f,22.5f,0.0f)// �ԗ��̕����]���̍ő�l

// �R���X�g���N�^
CVehicleBase::CVehicleBase(CModel* model, const CVector& pos, const CVector& rotation, ERoadType road)
	: CCharaBase(ETag::eVehicle, ETaskPriority::eVehicle)
	, mpModel(model)
	, mpBodyCol(nullptr)
	, mRoadType(road)
	, mCurrentRoadRotation(rotation)
	, mState(EState::eMove)
{
	// �o�H�Ǘ��N���X������Ȃ�ԗ��̎���p�̃m�[�h�𐶐�
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		mpNode0 = new CNavNode(Position());
		mpNode1 = new CNavNode(Position());
		mpNode2 = new CNavNode(Position());
		mpNode3 = new CNavNode(Position());
	}
	// �ŏ��̓m�[�h�𖳌�
	mpNode0->SetEnable(false);
	mpNode1->SetEnable(false);
	mpNode2->SetEnable(false);
	mpNode3->SetEnable(false);

	// �ŏ��͕`��A�X�V���Ȃ�
	SetEnable(false);
	SetShow(false);

	Position(pos);
	Rotation(rotation);
}

// �f�X�g���N�^
CVehicleBase::~CVehicleBase()
{
	// �R���C�_�\�̍폜
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpNavCol);
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
	return mIsMove;
}

// �Ԑ���ύX����
void CVehicleBase::ChangeRoad(bool& isEnd)
{
	isEnd = false;
	// ���ʂֈړ�����
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed();

	// ���̓����m��X����
	float xDistLeft = std::abs(CAR_LEFT_POS1.X() - CAR_LEFT_POS2.X());
	// �E�̓����m��X����
	float xDistRight = std::abs(CAR_RIGHT_POS1.X() - CAR_RIGHT_POS2.X());

	// ���̓����m�̒��ԓ_��X���W
	float leftMid = CAR_LEFT_POS1.X() + xDistLeft / 2;
	// �E�̓����m�̒��ԓ_��X���W
	float rightMid = CAR_RIGHT_POS1.X() - xDistLeft / 2;

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
		else if (Position().X() <= rightMid)
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
			mCurrentRoadRotation = CAR_RIGHT_ROTATION;
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

// �{�̃R���C�_�\���擾����
CCollider* CVehicleBase::GetBodyCol() const
{
	return mpBodyCol;
}

// �o�H�T���p�R���C�_�\���擾����
CCollider* CVehicleBase::GetNavCol() const
{
	return mpNavCol;
}

// �ړ�����
void CVehicleBase::UpdateMove()
{
	// �����Ă���
	mIsMove = true;
	// ���ʂֈړ�
	mMoveSpeed = VectorZ() * GetBaseMoveSpeed();
}

// ��~����
void CVehicleBase::UpdateStop()
{
	// �����Ă��Ȃ�
	mIsMove = false;
	// �ړ����x���[���ɂ���
	mMoveSpeed = CVector::zero;
}

// ��ꂽ����
// �ړ����~���ď��Ŏ��Ԃ��o������\��������
void CVehicleBase::UpdateBroken()
{
	// �����Ă��Ȃ�
	mIsMove = false;
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
void CVehicleBase::UpdateChangeRoad()
{
	// �����Ă���
	mIsMove = true;
	bool isEnd = false;
	// �Ԑ��ύX�ړ�
	ChangeRoad(isEnd);

	// true�Ȃ�΁A�Ԑ��ύX���I�����
	if (isEnd)
	{
		// �ړ���Ԃɖ߂�
		ChangeState(EState::eMove);
	}
}

// ��Ԑ؂�ւ�
void CVehicleBase::ChangeState(EState state)
{
	// �����Ȃ珈�����Ȃ�
	if (state == mState) return;

	mState = state;
}

#if _DEBUG
// ��Ԃ̕�������擾
std::string CVehicleBase::GetStateStr(EState state) const
{
	switch (state)
	{
	// ����
	case EState::eMove:			return "�ړ���";
	case EState::eStop:			return "��~��";
	case EState::eBroken:		return "���Ă���";
	case EState::eChangeRoad:	return "�Ԑ��ύX";

	// �g���b�N
	case EState::eCollect:		return "�����";
	default:					return "";
	}
}
#endif