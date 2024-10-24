//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// T�|�[�Y
	{ "Character\\Player\\anim\\idle.x",		true,	153.0f	},	// �ҋ@
	{ "Character\\Player\\anim\\walk.x",		true,	66.0f	},	// ���s
	{ "Character\\Player\\anim\\attack.x",		false,	91.0f	},	// �U��
	{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// �W�����v�J�n
	{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// �W�����v��
	{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// �W�����v�I��
};

#define PLAYER_HEIGHT 16.0f
#define MOVE_SPEED 0.375f * 2.0f
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f
#define CAPSULE_RADIUS 2.5f

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsWall(false)
	//, mIsInteract(false)
	, mpRideObject(nullptr)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mMotionBlurRemainTime(0.0f)
{
	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Player");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	//�t�B�[���h�ƃC���^���N�g�I�u�W�F�N�g�����Փ˔��������
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, CAPSULE_RADIUS, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT - CAPSULE_RADIUS, 0.0f),
		CAPSULE_RADIUS
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField,ELayer::eInteract });
	//�t�B�[���h�Ƃ����Փ˔���
	//mpColliderLine = new CColliderLine
	//(
	//	this, ELayer::ePlayer,
	//	CVector(0.0f, 0.0f, 0.0f),
	//	CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	//);
	//mpColliderLine->SetCollisionLayers({ ELayer::eField });

	//�C���^���N�g�I�u�W�F�N�g�Ƃ����Փ˔���
	//mpInteractColliderSphere = new CColliderSphere
	//(this, ELayer::ePlayer, 5, true);
	//mpInteractColliderSphere->SetCollisionLayers({ ELayer::eInteract });

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f),
		CQuaternion(0.0f, 90.0f, 0.0f).Matrix()
	);
}

CPlayer::~CPlayer()
{
	//if (mpColliderLine != nullptr)
	//{
	//	delete mpColliderLine;
	//	mpColliderLine = nullptr;
	//}
	if (mpColliderCapsule != nullptr)
	{
		delete mpColliderCapsule;
		mpColliderCapsule = nullptr;
	}
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

void CPlayer::SetIsWall(bool isWall)
{
	mIsWall = isWall;
}

// �ҋ@
void CPlayer::UpdateIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{
		// ���N���b�N�ōU����Ԃֈڍs
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			mState = EState::eAttack;
		}
		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
		//// F�L�[�ŃC���^���N�g
		//else if (CInput::PushKey('F'))
		//{
		//	//Interact�I�u�W�F�N�g�̃C���^���N�g�͈͓��Ȃ�
		//	if (GetInteract())
		//	{
		//		mState = EState::eInteract;
		//	}	
		//}
	}
}

// �U��
void CPlayer::UpdateAttack()
{
	// �U���A�j���[�V�������J�n
	ChangeAnimation(EAnimType::eAttack);
	// �U���I���҂���Ԃֈڍs
	mState = EState::eAttackWait;

	// �a��SE�̍Đ��ς݃t���O��������
	mIsPlayedSlashSE = false;
	// �a���G�t�F�N�g�̐����ς݃t���O��������
	mIsSpawnedSlashEffect = false;
}

// �U���I���҂�
void CPlayer::UpdateAttackWait()
{
	// �a��SE���Đ����Ă��Ȃ����A�A�j���[�V������25%�ȏ�i�s������A
	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
	{
		// �a��SE���Đ�
		mpSlashSE->Play();
		mIsPlayedSlashSE = true;
	}

	// �a���G�t�F�N�g�𐶐����Ă��Ȃ����A�A�j���[�V������35%�ȏ�i�s������A
	if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.35f)
	{
		// �a���G�t�F�N�g�𐶐����āA���ʕ����֔�΂�
		CSlash* slash = new CSlash
		(
			this,
			Position() + CVector(0.0f, 10.0f, 0.0f),
			VectorZ(),
			300.0f,
			100.0f
		);
		// �a���G�t�F�N�g�̐F�ݒ�
		slash->SetColor(CColor(0.15f, 0.5f, 0.5f));

		mIsSpawnedSlashEffect = true;
	}

	// �U���A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}

// �W�����v�J�n
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	mState = EState::eJump;

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// �W�����v��
void CPlayer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// �W�����v�I��
void CPlayer::UpdateJumpEnd()
{
	// �W�����v�A�j���[�V�������I�����A
	// �n�ʂɐڒn������A�ҋ@��Ԃ֖߂�
	if (IsAnimationFinished() && mIsGrounded)
	{
		mState = EState::eIdle;
	}
}

//// �C���^���N�g��
//void CPlayer::UpdateInteract()
//{
//	if (!GetInteract())
//	{
//		mState = EState::eIdle;
//	}
//}

// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
CVector CPlayer::CalcMoveVec()
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
		// TODO�F�ǂƂԂ����Ă���Ƃ��ǂ���������ւ̈ړ��𐧌�����
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

		// �ǂƐڐG���Ă�����ǂ̖@���Ƌt�����̈ړ��x�N�g���𖳌��ɂ���
		if (mIsWall == true)
		{
			// TODO
			// �ǂ̖@����0�łȂ��Ƃ��ړ��ƕ����������Ȃ�AmIsWall��false
			if (mWallNormal.X() == moveSide*input.X() && mWallNormal.X() != 0.0f)
			{
				mIsWall = false;
			}
			if (mWallNormal.Z() == input.Y() && mWallNormal.Z() != 0.0f)
			{
				mIsWall = false;
			}
			

			// �ǂ̖@���̋t�x�N�g�����擾
			CVector wallInverseNormal = -mWallNormal;

			// �t�x�N�g�����ړ��x�N�g����+X�����̏ꍇ�i�E�j
			if (wallInverseNormal.X() > 0.0f && input.X() > 0.0f)
			{
				input.X(0.0f);
			}
			// �t�x�N�g�����ړ��x�N�g����-X�����̏ꍇ�i���j
			else if (wallInverseNormal.X() < 0.0f && input.X() < 0.0f)
			{
				input.X(0.0f);
			}

			// �t�x�N�g�����ړ��x�N�g����+Z�����̏ꍇ�i��j	
			if (wallInverseNormal.Z() > 0.0f && input.Y() > 0.0f)
			{
				input.Y(0.0f);
			}
			// �t�x�N�g�����ړ��x�N�g����-Z�����̏ꍇ�i�O�j
			else if (wallInverseNormal.Z() < 0.0f && input.Y() < 0.0f)
			{
				input.Y(0.0f);
			}
		}

		// ���߂��e�����̈ړ��x�N�g������A
		// �ŏI�I�ȃv���C���[�̈ړ��x�N�g�������߂�
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}

// �ړ��̍X�V����
void CPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED;

		// �ҋ@��Ԃł���΁A���s�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eWalk);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eIdle);
		}
	}
}

// ���[�V�����u���[�̍X�V����
void CPlayer::UpdateMotionBlur()
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
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
		case EState::eIdle:
			UpdateIdle();
			break;
		// �U��
		case EState::eAttack:
			UpdateAttack();
			break;
		// �U���I���҂�
		case EState::eAttackWait:
			UpdateAttackWait();
			break;
		// �W�����v�J�n
		case EState::eJumpStart:
			UpdateJumpStart();
			break;
		// �W�����v��
		case EState::eJump:
			UpdateJump();
			break;
		// �W�����v�I��
		case EState::eJumpEnd:
			UpdateJumpEnd();
			break;
		//// �C���^���N�g��
		//case EState::eInteract:
		//	UpdateInteract();
	}

	// �ҋ@���ƃW�����v���́A�ړ��������s��
	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// �ړ�
	Position(Position() + moveSpeed);

	// �v���C���[���ړ������֌�����
	CVector current = VectorZ();
	CVector target = moveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// �E�N���b�N�Œe�۔���
	if (CInput::PushKey(VK_RBUTTON))
	{
		// �e�ۂ𐶐�
		new CBullet
		(
			// ���ˈʒu
			Position() + CVector(0.0f, 10.0f, 0.0f) + VectorZ() * 20.0f,
			VectorZ(),	// ���˕���
			1000.0f,	// �ړ�����
			1000.0f		// �򋗗�
		);
	}

	// �uE�v�L�[�ŉ��̔��˂��I���I�t����
	if (CInput::PushKey('E'))
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
		else
		{
			mpFlamethrower->Stop();
		}
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
	CDebugPrint::Print("State:%d\n", mState);

	mIsGrounded = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// �Փˏ���
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
				mGroundNormal = hit.adjust.Normalized();

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
			// ���ł��V��ł��Ȃ��Ȃ�ǂɏՓ˂���
			else
			{
				CVector horizontalNormal = normal;
				horizontalNormal.Y(0.0f); // �����ʂ̖@��

				if (horizontalNormal.LengthSqr() > 0.0f)
				{
					horizontalNormal = horizontalNormal.Normalized();

					// �������̑��x�𒲐�����
					if (fabs(CVector::Dot(horizontalNormal, mMoveSpeed)) > 0.0f)
					{
						//mMoveSpeed = CVector(0.0f, 0.0f, 0.0f);
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
void CPlayer::Render()
{
	CXCharacter::Render();
}

////mIsInteract�̒l��ݒ�
//void CPlayer::SetInteract(bool interact)
//{
//	mIsInteract = interact;
//}
////mIsInteract�̒l��Ԃ�
//bool CPlayer::GetInteract()
//{
//	return mIsInteract;
//}
//
//void CPlayer::SetInteractObject(std::string interactName)
//{
//	if (interactName == "eComputer")
//	{
//		mInteractObject = EInteractObject::eComputer;
//	}
//	else if (interactName == "eDoor")
//	{
//		mInteractObject = EInteractObject::eDoor;
//	}
//	else
//	{
//		mInteractObject = EInteractObject::None;
//	}
//}
//
//
//
//std::string CPlayer::GetInteractObject()
//{
//	switch (mInteractObject)
//	{
//	case EInteractObject::eComputer:
//		return "eComputer";
//
//	case EInteractObject::eDoor:
//		return "eDoor";
//	
//	default:
//		return "None";
//	}
//}

