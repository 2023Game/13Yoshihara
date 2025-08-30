#include "CSwitchMoveWall.h"
#include "CColliderMesh.h"
#include "Maths.h"
#include "CConnectPointManager.h"

// �R���X�g���N�^
CSwitchMoveWall::CSwitchMoveWall(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime, bool isCrushed)
	: mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mpCrushedCol(nullptr)
	, mIsOpen(false)
{
	mpModel = model;

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

	// �������W��ݒ�
	Position(mDefaultPos);
	Scale(scale);
}

// �f�X�g���N�^
CSwitchMoveWall::~CSwitchMoveWall()
{
	SAFE_DELETE(mpCrushedCol);
}

// �J���Ă��邩
bool CSwitchMoveWall::IsOpen() const
{
	return mIsOpen;
}

// �J���Ă��邩��ݒ�
void CSwitchMoveWall::SetIsOpen(bool enable)
{
	mIsOpen = enable;
	// �I���I�t��؂�ւ���
	SetOnOff(enable);
}

// �R���C�_�[�𐶐�
void CSwitchMoveWall::CreateCol()
{
	mpCol = new CColliderMesh(this, ELayer::eWall, mpModel, true);

	// �ڑ����̊Ǘ��N���X
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// �R���C�_�[��ǉ�
	pointMgr->AddCollider(mpCol);
	// ���݂̃J����
	CCamera* camera = CCamera::CurrentCamera();
	// �R���C�_�[��ǉ�
	camera->AddCollider(mpCol);
}

// ��p���Ă��Ȃ����̏���
void CSwitchMoveWall::UpdateOff()
{
	// �o�ߎ��Ԃ�0�b�Ȃ珈�����Ȃ�
	if (mElapsedTime == 0.0f) return;

	// �ړ�����
	Move();

	// �o�ߎ��Ԃ�����
	mElapsedTime -= Times::DeltaTime();
	// �o�ߎ��Ԃ�0�ȉ��Ȃ�
	if (mElapsedTime <= 0.0f)
	{
		// �J���Ă��Ȃ�
		mIsOpen = false;
		// �o�ߎ��Ԃ�0
		mElapsedTime = 0.0f;
		// ���W���X�V
		Move();
	}
}

// ��p���Ă��鎞�̏���
void CSwitchMoveWall::UpdateOn()
{
	// �ړ����Ԃ��o�߂��Ă����珈�����Ȃ�
	if (mElapsedTime == mMoveTime) return;

	// �J���Ă���
	mIsOpen = true;

	// �ړ�����
	Move();

	// ���Ԍo��
	mElapsedTime += Times::DeltaTime();
	// �ړ����Ԃ��o�߂��Ă�����
	if (mElapsedTime >= mMoveTime)
	{
		// �o�ߎ��Ԃ̒l���ړ����Ԃɂ���
		mElapsedTime = mMoveTime;
		// ���W���X�V
		Move();
	}
}

// �ړ��̏���
void CSwitchMoveWall::Move()
{
	// ���Ԃ��i�񂾊���
	float per = mElapsedTime / mMoveTime;
	// ���W�����߂�
	Position(mDefaultPos + mMoveVec * per);
}

// ��Ԃ�ύX
void CSwitchMoveWall::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;

	// ��O�̏�Ԃ��X�V
	mPreMoveState = mMoveState;
	// ��Ԃ�ύX
	mMoveState = state;
	mElapsedTime = 0.0f;
}

// ��Ԃ�ݒ�
void CSwitchMoveWall::SetState(EMoveState state)
{
	mMoveState = state;
}

// ��Ԃ��擾
EMoveState CSwitchMoveWall::GetState() const
{
	return mMoveState;
}

// �O��̏�Ԃ�ݒ�
void CSwitchMoveWall::SetPreState(EMoveState state)
{
	mPreMoveState = state;
}

// �O��̏�Ԃ��擾
EMoveState CSwitchMoveWall::GetPreState() const
{
	return mPreMoveState;
}

// �o�ߎ��Ԃ�ݒ�
void CSwitchMoveWall::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// �o�ߎ��Ԃ��擾
float CSwitchMoveWall::GetElapsedTime() const
{
	return mElapsedTime;
}