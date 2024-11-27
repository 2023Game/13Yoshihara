//�v���C���[�N���X�̃C���N���[�h
#include "CPlayerBase.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"

// �v���C���[�̃C���X�^���X
CPlayerBase* CPlayerBase::spInstance = nullptr;



#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f

#define CAPSULE_RADIUS_TRASH 2.5f

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

// �R���X�g���N�^
CPlayerBase::CPlayerBase(float capsuleRadius, float playerHeight)
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsWall(false)
	, mpRideObject(nullptr)
	, mMotionBlurRemainTime(0.0f)
	, mIsDamage(false)
{
	spInstance = this;

	//�t�B�[���h�Ƃ����Փ˔��������
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, capsuleRadius, 0.0f),
		CVector(0.0f, playerHeight - capsuleRadius, 0.0f),
		capsuleRadius
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField });
}

CPlayerBase::~CPlayerBase()
{
}

CPlayerBase* CPlayerBase::Instance()
{
	return spInstance;
}

//// �U��
//void CPlayerBase::UpdateAttack()
//{
//	// �U���A�j���[�V�������J�n
//	ChangeAnimation(EAnimType::eAttack);
//	// �U���I���҂���Ԃֈڍs
//	mState = EState::eAttackWait;
//
//	// �a��SE�̍Đ��ς݃t���O��������
//	mIsPlayedSlashSE = false;
//	// �a���G�t�F�N�g�̐����ς݃t���O��������
//	mIsSpawnedSlashEffect = false;
//}
//
//// �U���I���҂�
//void CPlayerBase::UpdateAttackWait()
//{
//	// �a��SE���Đ����Ă��Ȃ����A�A�j���[�V������25%�ȏ�i�s������A
//	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
//	{
//		// �a��SE���Đ�
//		mpSlashSE->Play();
//		mIsPlayedSlashSE = true;
//	}
//
//	// �a���G�t�F�N�g�𐶐����Ă��Ȃ����A�A�j���[�V������35%�ȏ�i�s������A
//	if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.35f)
//	{
//		// �a���G�t�F�N�g�𐶐����āA���ʕ����֔�΂�
//		CSlash* slash = new CSlash
//		(
//			this,
//			Position() + CVector(0.0f, 10.0f, 0.0f),
//			VectorZ(),
//			300.0f,
//			100.0f
//		);
//		// �a���G�t�F�N�g�̐F�ݒ�
//		slash->SetColor(CColor(0.15f, 0.5f, 0.5f));
//
//		mIsSpawnedSlashEffect = true;
//	}
//
//	// �U���A�j���[�V�������I��������A
//	if (IsAnimationFinished())
//	{
//		// �ҋ@��Ԃֈڍs
//		mState = EState::eIdle;
//		ChangeAnimation(EAnimType::eIdle);
//	}
//}


// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
CVector CPlayerBase::CalcMoveVec()
{
	CVector move = CVector::zero;

	// �L�[�̓��̓x�N�g�����擾
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// ���̓x�N�g���̒����œ��͂���Ă��邩����
	if (input.LengthSqr() > 0.0f)
	{
		// ������x�N�g��(�ݒu���Ă���ꍇ�́A�n�ʂ̖@��)
		CVector up = mIsGrounded ? mGroundNormal : CVector::up;
		// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		camForward.Y(0.0f);
		camForward.Normalize();
		// �J�����̐��ʕ����x�N�g���Ə�����x�N�g���̊O�ς���
		// �������̈ړ��x�N�g�������߂�
		CVector moveSide = CVector::Cross(up, camForward);
		// �������̈ړ��x�N�g���Ə�����x�N�g���̊O�ς���
		// ���ʕ����̈ړ��x�N�g�������߂�
		CVector moveForward = CVector::Cross(moveSide, up);

		// ���߂��e�����̈ړ��x�N�g������A
		// �ŏI�I�ȃv���C���[�̈ړ��x�N�g�������߂�
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}

// ���[�V�����u���[�̍X�V����
void CPlayerBase::UpdateMotionBlur()
{
	// ���[�V�����u���[�̎c�莞�Ԃ��c���Ă��Ȃ���΁A�������Ȃ�
	if (mMotionBlurRemainTime <= 0.0f) return;
	// ���݂̃J�������擾���A���݂��Ȃ���Ώ������Ȃ�
	CCamera* currentCamera = CCamera::CurrentCamera();
	if (currentCamera == nullptr) return;

	// �J�����̌����Ɣ��Ε����փu���[���|���邽�߁A
	// ���]�����J�����̌������擾
	CVector camDir = -currentCamera->VectorZ();

	// �c�莞�Ԃ���o�ߎ��Ԃ̊������擾�i�o�ߎ��Ԃ̊��� = 1 - �c�莞�Ԃ̊����j
	float percent = 1.0f - mMotionBlurRemainTime / MOTION_BLUR_TIME;
	// �u���[�̕����T�C���J�[�u�Ōo�ߎ��Ԃɍ��킹�đ���������
	float ratio = sinf(M_PI * percent);
	float width = MOTION_BLUR_WIDTH * ratio;

	// ���[�V�����u���[�̃p�����[�^��ݒ�
	System::SetMotionBlur(camDir, width, MOTION_BLUR_COUNT);

	// �c�莞�Ԃ��o�ߎ��ԕ�����������
	mMotionBlurRemainTime -= Times::DeltaTime();
	// �c�莞�Ԃ��Ȃ��Ȃ�΁A
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// ���[�V�����u���[���I�t�ɂ���
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
	}
}

// �X�V
void CPlayerBase::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// �ړ�
	Position(Position() + moveSpeed);

	// �U�����󂯂Ă��Ȃ�����
	if (!mIsDamage)
	{
		// �v���C���[���ړ������֌�����
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));
	}


	// �uP�v�L�[����������A�Q�[�����I��
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// �uB�v�L�[����������A���[�V�����u���[�J�n
	if (CInput::PushKey('B'))
	{
		// ���[�V�����u���[���|���Ă���Œ��ł���΁A
		// �V�������[�V�����u���[���|�������Ȃ�
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
		}
	}

	// ���[�V�����u���[�̍X�V����
	UpdateMotionBlur();

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("Wall:%s\n", mIsWall ? "true" : "false");

	mIsGrounded = false;
	mIsWall = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// �Փˏ���
void CPlayerBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderCapsule)
	{
		// �Փ˂������肪�t�B�[���h�I�u�W�F�N�g�̏ꍇ
		if (other->Layer() == ELayer::eField)
		{
			// ���e�͈�
			const float epsilon = 0.001f;

			CVector adjust = hit.adjust;

			// �Փ˂����n�ʂ������V�䂩����ςŔ���
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);

			// ���ς̌��ʂ�epsilon�ȏ�Ȃ�A���ƏՓ˂���
			if (dot >= epsilon)
			{
				// �����Ȃǂŏ��ɏォ��Փ˂������i���ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// �ڒn����
				mIsGrounded = true;
				// �ڒn�����n�ʂ̖@�����L�����Ă���
				mGroundNormal = normal;

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}

				// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
				adjust.X(0.0f);
				adjust.Z(0.0f);
			}
			// ���ς̌��ʂ�-epsilon��菬������΁A�V��ƏՓ˂���
			else if (dot < -epsilon)
			{
				// �W�����v�ȂǂœV��ɉ�����Փ˂������i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
			// TODO:�ǂɈړ��������Ă���Ƃ��k����̂Ō�ŏC��
			// ���ł��V��ł��Ȃ��Ȃ�ǂɏՓ˂���
			else
			{
				CVector horizontalNormal = normal;
				horizontalNormal.Y(0.0f); // �����ʂ̖@��

					// X�����́{�։����߂��ǂȂ�|���ɂ���̂�
				if (horizontalNormal.X() > 0.0f)
				{
					// X�ړ����|�Ȃ�0�ɂ���
					if (mMoveSpeed.X() < 0.0f)
					{
						mMoveSpeed.X(0.0f);
					}
				}
				// X�����́|�։����߂��ǂȂ�{���ɂ���̂�
				else if (horizontalNormal.X() < 0.0f)
				{
					// X�ړ����{�Ȃ�0�ɂ���
					if (mMoveSpeed.X() > 0.0f)
					{
						mMoveSpeed.X(0.0f);
					}
				}

				// Z�����́{�։����߂��ǂȂ�|���ɂ���̂�
				if (horizontalNormal.Z() > 0.0f)
				{
					// Z�ړ����|�Ȃ�0�ɂ���
					if (mMoveSpeed.Z() < 0.0f)
					{
						mMoveSpeed.Z(0.0f);
					}
				}
				// Z�����́|�։����߂��ǂȂ�{���ɂ���̂�
				else if (horizontalNormal.Z() < 0.0f)
				{
					// Z�ړ����{�Ȃ�0�ɂ���
					if (mMoveSpeed.Z() > 0.0f)
					{
						mMoveSpeed.Z(0.0f);
					}
				}

				// �ǂɐڐG����
				mIsWall = true;
				// �ڐG�����ǂ̖@�����L��
				mWallNormal = horizontalNormal;

				// �c�����̒����l��0�ɂ���
				adjust.Y(0.0f);
			}

			// �ʒu�̒���
			Position(Position() + adjust * hit.weight);
		}
	}
}

// �`��
void CPlayerBase::Render()
{
	CXCharacter::Render();
}
