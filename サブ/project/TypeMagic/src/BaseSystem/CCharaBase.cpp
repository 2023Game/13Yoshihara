#include "CCharaBase.h"

CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsGravity(true)
	, mIsMoveDir(true)
	, mpRideObject(nullptr)
{
}

CCharaBase::~CCharaBase()
{
}

// �X�V
void CCharaBase::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// �d�͂��|����Ȃ�
	if (mIsGravity)
	{
		//mMoveSpeedY -= GRAVITY;
	}
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	// �ړ�������ݒ�
	mMoveDir = moveSpeed.Normalized();

	// �ړ�
	Position(Position() + moveSpeed);

	// �U�����󂯂Ă��Ȃ�������
	// �ړ������������ݒ肪�I���̎�
	if (!mIsDamage &&
		mIsMoveDir)
	{
		// �v���C���[���ړ������֌�����
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));
	}
}

// �ړ��������擾����
CVector CCharaBase::GetMoveDir()
{
	return mMoveDir;
}
