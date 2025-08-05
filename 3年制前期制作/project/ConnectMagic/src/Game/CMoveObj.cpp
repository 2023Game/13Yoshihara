#include "CMoveObj.h"
#include "Maths.h"
#include "CConnectPointManager.h"

// �ꎞ��~�̎���
#define STOP_TIME 0.5f

CMoveObj::CMoveObj(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale,
	const CVector& move, float moveTime,
	ELayer layer)
	: mpModel(model)
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mState(EMoveState::eStop)
	, mPreState(EMoveState::eBack)
{
	// �{�̂̃R���C�_�[
	mpColliderMesh = new CColliderMesh(this, layer, mpModel, true);
	// �v���C���[�����܂ꂽ���p�̃R���C�_�[
	mpCrushedCol = new CColliderMesh(this, ELayer::eCrushed, col, true);
	// �v���C���[�Ƃ����Փ�
	mpCrushedCol->SetCollisionLayers({ ELayer::ePlayer });

	// �ڑ����̊Ǘ��N���X
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// �R���C�_�[��ǉ�
	pointMgr->AddCollider(mpColliderMesh);
	// ���݂̃J����
	CCamera* camera = CCamera::CurrentCamera();
	// �R���C�_�[��ǉ�
	camera->AddCollider(mpColliderMesh);

	Position(mDefaultPos);
	Scale(scale);
}

CMoveObj::~CMoveObj()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CMoveObj::Update()
{
	mElapsedTime += Times::DeltaTime();

	switch (mState)
	{
	case CMoveObj::EMoveState::eStop:
		UpdateStop();
		break;
	case CMoveObj::EMoveState::eGo:
		UpdateGo();
		break;
	case CMoveObj::EMoveState::eBack:
		UpdateBack();
		break;
	}
}

void CMoveObj::Render()
{
	mpModel->Render(Matrix());
}

// ��Ԃ�ύX
void CMoveObj::ChangeState(EMoveState state)
{
	if (mState == state) return;
	
	// ��O�̏�Ԃ��X�V
	mPreState = mState;
	// ��Ԃ�ύX
	mState = state;
	mElapsedTime = 0.0f;
}

// �~�܂��Ă���Ƃ��̍X�V
void CMoveObj::UpdateStop()
{
	// �ꎞ��~�̎��Ԃ��o�߂�����
	if (mElapsedTime >= STOP_TIME)
	{
		// ��O���i�ޏ�Ԃ�������
		if (mPreState == EMoveState::eGo)
		{
			// �߂��ԂɕύX
			ChangeState(EMoveState::eBack);
		}
		// �߂��ԂȂ�
		else if (mPreState == EMoveState::eBack)
		{
			// �i�ޏ�Ԃ�
			ChangeState(EMoveState::eGo);
		}
	}
}

// �i��ł���Ƃ��̍X�V
void CMoveObj::UpdateGo()
{
	float per = mElapsedTime / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// ���Ԃ��o�߂�����ꎞ��~��Ԃ�
	if (mElapsedTime >= mMoveTime)
	{
		ChangeState(EMoveState::eStop);
	}
}

// �߂��Ă���Ƃ��̍X�V
void CMoveObj::UpdateBack()
{
	float per = (mMoveTime - mElapsedTime) / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// ���Ԃ��o�߂�����ꎞ��~��Ԃ�
	if (mElapsedTime >= mMoveTime)
	{
		ChangeState(EMoveState::eStop);
	}
}
