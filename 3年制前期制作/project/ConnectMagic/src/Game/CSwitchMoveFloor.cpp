#include "CSwitchMoveFloor.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"

// �ꎞ��~�̎���
#define STOP_TIME 0.5f

// �R���X�g���N�^
CSwitchMoveFloor::CSwitchMoveFloor(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime, bool isCrushed)
	: CSwitchObject()
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mMoveState(EMoveState::eStop)
	, mPreMoveState(EMoveState::eBack)
	, mpCrushedCol(nullptr)
{
	mpModel = model;

	// �^�O����邱�Ƃ��o����I�u�W�F�N�g�ɐݒ�
	SetTag(ETag::eRideableObject);

	// �R���C�_�[�𐶐�
	CreateCol();
	// �v���C���[���󂷂Ȃ�
	if (isCrushed)
	{
		// �v���C���[�����܂ꂽ���p�̃R���C�_�[
		mpCrushedCol = new CColliderMesh(this, ELayer::eCrushed, col, true);
		// �v���C���[�Ƃ����Փ�
		mpCrushedCol->SetCollisionLayers({ ELayer::ePlayer });
	}

	// �������W�̐ݒ�
	Position(mDefaultPos);
	Scale(scale);
}

// �f�X�g���N�^
CSwitchMoveFloor::~CSwitchMoveFloor()
{
	SAFE_DELETE(mpCrushedCol);
}

// �R���C�_�[�𐶐�
void CSwitchMoveFloor::CreateCol()
{
	// �{�̂̃R���C�_�[
	mpCol = new CColliderMesh(this, ELayer::eGround, mpModel, true);

	// �ڑ����̊Ǘ��N���X
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// �R���C�_�[��ǉ�
	pointMgr->AddCollider(mpCol);
	// ���݂̃J����
	CCamera* camera = CCamera::CurrentCamera();
	// �R���C�_�[��ǉ�
	camera->AddCollider(mpCol);
}

// ��p���Ă��鎞�̏���
void CSwitchMoveFloor::UpdateOn()
{
	mElapsedTime += Times::DeltaTime();

	switch (mMoveState)
	{
	case EMoveState::eStop:
		UpdateStop();
		break;
	case EMoveState::eGo:
		UpdateGo();
		break;
	case EMoveState::eBack:
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

// ��Ԃ�ݒ�
void CSwitchMoveFloor::SetState(EMoveState state)
{
	mMoveState = state;
}

// ��Ԃ��擾
EMoveState CSwitchMoveFloor::GetState() const
{
	return mMoveState;
}

// �O��̏�Ԃ�ݒ�
void CSwitchMoveFloor::SetPreState(EMoveState state)
{
	mPreMoveState = state;
}

// �O��̏�Ԃ��擾
EMoveState CSwitchMoveFloor::GetPreState() const
{
	return mPreMoveState;
}

// �o�ߎ��Ԃ�ݒ�
void CSwitchMoveFloor::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// �o�ߎ��Ԃ��擾
float CSwitchMoveFloor::GetElapsedTime() const
{
	return mElapsedTime;
}