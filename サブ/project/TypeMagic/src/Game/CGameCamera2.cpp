#include "CGameCamera2.h"
#include "CInput.h"
#include "Maths.h"

// �J�����̉�]���x
#define ROTATE_SPEED 1.0f
// �J�����̏㉺��]�͈̔�
#define ROTATE_RANGE_X 45.0f
// �J�����̃I�t�Z�b�g���WZ
#define EYE_OFFSET_POS_Z 100.0f

// �R���X�g���N�^
CGameCamera2::CGameCamera2(const CVector& eye, const CVector& center, bool isMainCamera)
	: CCamera(eye, center, isMainCamera)
	, mFollowDefaultEyeVec(CVector::forward)
	, mRotateAngle(CVector::zero)
	, mpLockOnTarget(nullptr)
{
}

// �f�X�g���N�^
CGameCamera2::~CGameCamera2()
{
}

void CGameCamera2::SetFollowTargetTf(CTransform* target)
{
	mFollowTargetTf = target;
	if (mFollowTargetTf != nullptr)
	{
		mFollowDefaultEyeVec = mTargetEye - mAt;
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

void CGameCamera2::LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye)
{
	CCamera::LookAt(eye, at, up, updateTargetEye);
	if (mFollowTargetTf != nullptr)
	{
		if (updateTargetEye)
		{
			mFollowDefaultEyeVec = mTargetEye - mAt;
		}
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

// �X�V
void CGameCamera2::Update()
{
	// �Ǐ]����^�[�Q�b�g�ƃ��b�N�I���^�[�Q�b�g���ݒ肳��Ă����
	if (mFollowTargetTf != nullptr &&
		mpLockOnTarget != nullptr)
	{
		// ���b�N�I���^�[�Q�b�g����Ǐ]�^�[�Q�b�g�ւ̕���
		CVector dir = mFollowTargetTf->Position() - mpLockOnTarget->Position();
		dir.Normalize();

		// �����_�͒Ǐ]�^�[�Q�b�g
		mAt = mFollowTargetTf->Position()+ mFollowOffsetPos;
		// ���_�͒����_��dir�����ɂ��炵�����W
		CVector eye = mAt + dir * EYE_OFFSET_POS_Z;
		// ��Ԃ����l��ݒ�
		mTargetEye = CVector::Lerp(mEye, eye, 0.1f);
		mEye = mTargetEye;
	}
	// �Ǐ]�^�[�Q�b�g�����Ȃ�
	else if (mFollowTargetTf != nullptr)
	{
		// ���b�N�I���^�[�Q�b�g����Ǐ]�^�[�Q�b�g�ւ̕���
		CVector dir = mFollowTargetTf->Position() - CVector::zero;
		dir.Normalize();

		// �����_�͒Ǐ]�^�[�Q�b�g
		mAt = mFollowTargetTf->Position() + mFollowOffsetPos;
		// ���_�͒����_��dir�����ɂ��炵�����W
		CVector eye = mAt + dir * EYE_OFFSET_POS_Z;
		// ��Ԃ����l��ݒ�
		mTargetEye = CVector::Lerp(mEye, eye, 0.1f);
		mEye = mTargetEye;

		//// �����_�͒Ǐ]�^�[�Q�b�g
		//mAt = mFollowTargetTf->Position() + mFollowOffsetPos;
		//// ���_�͒Ǐ]�^�[�Q�b�g�̌���ɂ��炵�����W
		//CVector eye = mAt + (-mFollowTargetTf->VectorZ() * EYE_OFFSET_POS_Z);
		//// ��Ԃ����l��ݒ�
		//mTargetEye = CVector::Lerp(mEye, eye, 0.1f);
		//mEye = mTargetEye;
	}

#if _DEBUG
	CDebugPrint::Print("Angle:%f, %f, %f", mRotateAngle.X(), mRotateAngle.Y(), mRotateAngle.Z());
#endif
	// �ݒ肳��Ă���R���C�_�[�ƏՓ˂���ꍇ�́A
	// �J�����̈ʒu�������o��
	ApplyCollision();

	// ���_�A�����_�A��x�N�g������e�s����X�V
	LookAt(mEye, mAt, mUp, false);

	// �J�����̃x�[�X�̍X�V����
	//CCamera::Update();
}

// ��]�p�x��ݒ�
void CGameCamera2::SetRotateAngle(CVector angle)
{
	mRotateAngle += angle;
}

// ���b�N�I���^�[�Q�b�g��ݒ�
void CGameCamera2::SetLockOnTarget(CTransform* target)
{
	mpLockOnTarget = target;
}

// ���b�N�I���^�[�Q�b�g���擾
CTransform* CGameCamera2::GetLockOnTarget() const
{
	return mpLockOnTarget;
}

