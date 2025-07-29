#include "CSwitchMoveFloor.h"
#include "CColliderMesh.h"

// �R���X�g���N�^
CSwitchMoveFloor::CSwitchMoveFloor(CModel* model,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime)
	: CSwitchObject()
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
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
