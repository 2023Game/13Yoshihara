#include "CSwitchMoveWall.h"
#include "CColliderMesh.h"
#include "Maths.h"

// �R���X�g���N�^
CSwitchMoveWall::CSwitchMoveWall(CModel* model,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime)
	: mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
{
	mpModel = model;

	// �R���C�_�[�𐶐�
	CreateCol();

	// �������W��ݒ�
	Position(mDefaultPos);
	Scale(scale);
}

// �f�X�g���N�^
CSwitchMoveWall::~CSwitchMoveWall()
{
}

// �R���C�_�[�𐶐�
void CSwitchMoveWall::CreateCol()
{
	mpCol = new CColliderMesh(this, ELayer::eWall, mpModel, true);
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
		// ���W���X�V
		Move();
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
