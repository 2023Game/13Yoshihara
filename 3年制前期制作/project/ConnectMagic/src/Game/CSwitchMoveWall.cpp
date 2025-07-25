#include "CSwitchMoveWall.h"
#include "CColliderMesh.h"
#include "Maths.h"

// �R���X�g���N�^
CSwitchMoveWall::CSwitchMoveWall(CModel* model, CModel* colModel,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime)
	: mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
{
	mpModel = model;

	mpCol = new CColliderMesh(this, ELayer::eWall, colModel, true);

	Position(mDefaultPos);
	Scale(scale);
}

// �f�X�g���N�^
CSwitchMoveWall::~CSwitchMoveWall()
{
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
		// �o�ߎ��Ԃ�0
		mElapsedTime = 0.0f;
	}
}

// ��p���Ă��鎞�̏���
void CSwitchMoveWall::UpdateOn()
{
	// �ړ����Ԃ��o�߂��Ă����珈�����Ȃ�
	if (mElapsedTime == mMoveTime) return;

	// �ړ�����
	Move();

	// ���Ԍo��
	mElapsedTime += Times::DeltaTime();
	// �ړ����Ԃ��o�߂��Ă�����
	if (mElapsedTime >= mMoveTime)
	{
		// �o�ߎ��Ԃ̒l���ړ����Ԃɂ���
		mElapsedTime = mMoveTime;
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
