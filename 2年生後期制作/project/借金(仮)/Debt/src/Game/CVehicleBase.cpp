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