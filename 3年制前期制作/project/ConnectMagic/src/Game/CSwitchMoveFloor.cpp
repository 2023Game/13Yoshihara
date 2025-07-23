#include "CSwitchMoveFloor.h"
#include "CColliderMesh.h"

// �R���X�g���N�^
CSwitchMoveFloor::CSwitchMoveFloor(CModel* model, CModel* colModel,
	CVector& pos, CVector& move, float moveTime)
	: CSwitchObject()
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
{
	mpModel = model;

	mpCol = new CColliderMesh(this, ELayer::eGround, colModel, true);

	Position(mDefaultPos);
}

// �f�X�g���N�^
CSwitchMoveFloor::~CSwitchMoveFloor()
{
}

// ��p���Ă��Ȃ����̏���
void CSwitchMoveFloor::UpdateOff()
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
void CSwitchMoveFloor::UpdateOn()
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
void CSwitchMoveFloor::Move()
{
	// ���Ԃ��i�񂾊���
	float per = mElapsedTime / mMoveTime;
	// ���W�����߂�
	Position(mDefaultPos + mMoveVec * per);
}
