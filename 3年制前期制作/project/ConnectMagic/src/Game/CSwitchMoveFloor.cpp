#include "CSwitchMoveFloor.h"
#include "CColliderMesh.h"

// �ꎞ��~�̎���
#define STOP_TIME 0.5f

// �R���X�g���N�^
CSwitchMoveFloor::CSwitchMoveFloor(CModel* model,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime)
	: CSwitchObject()
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mMoveState(EMoveState::eStop)
	, mPreMoveState(EMoveState::eBack)
{
	mpModel = model;

	// �^�O����邱�Ƃ��o����I�u�W�F�N�g�ɐݒ�
	SetTag(ETag::eRideableObject);

	// �R���C�_�[�𐶐�
	CreateCol();

	// �������W�̐ݒ�
	Position(mDefaultPos);
	Scale(scale);
}

// �f�X�g���N�^
CSwitchMoveFloor::~CSwitchMoveFloor()
{
}

// �R���C�_�[�𐶐�
void CSwitchMoveFloor::CreateCol()
{
	mpCol = new CColliderMesh(this, ELayer::eGround, mpModel, true);
}

// ��p���Ă��鎞�̏���
void CSwitchMoveFloor::UpdateOn()
{
	mElapsedTime += Times::DeltaTime();

	switch (mMoveState)
	{
	case CSwitchMoveFloor::EMoveState::eStop:
		UpdateStop();
		break;
	case CSwitchMoveFloor::EMoveState::eGo:
		UpdateGo();
		break;
	case CSwitchMoveFloor::EMoveState::eBack:
		UpdateBack();
		break;
	}
}

// ��Ԃ�ύX
void CSwitchMoveFloor::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;

	// ��O�̏�Ԃ��X�V
	mPreMoveState = mMoveState;
	// ��Ԃ�ύX
	mMoveState = state;
	mElapsedTime = 0.0f;
}

// �~�܂��Ă���Ƃ��̍X�V
void CSwitchMoveFloor::UpdateStop()
{
	// �ꎞ��~�̎��Ԃ��o�߂�����
	if (mElapsedTime >= STOP_TIME)
	{
		// ��O���i�ޏ�Ԃ�������
		if (mPreMoveState == EMoveState::eGo)
		{
			// �߂��ԂɕύX
			ChangeMoveState(EMoveState::eBack);
		}
		// �߂��ԂȂ�
		else if (mPreMoveState == EMoveState::eBack)
		{
			// �i�ޏ�Ԃ�
			ChangeMoveState(EMoveState::eGo);
		}
	}
}

// �i��ł���Ƃ��̍X�V
void CSwitchMoveFloor::UpdateGo()
{
	float per = mElapsedTime / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// ���Ԃ��o�߂�����ꎞ��~��Ԃ�
	if (mElapsedTime >= mMoveTime)
	{
		ChangeMoveState(EMoveState::eStop);
	}
}

// �߂��Ă���Ƃ��̍X�V
void CSwitchMoveFloor::UpdateBack()
{
	float per = (mMoveTime - mElapsedTime) / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// ���Ԃ��o�߂�����ꎞ��~��Ԃ�
	if (mElapsedTime >= mMoveTime)
	{
		ChangeMoveState(EMoveState::eStop);
	}
}
