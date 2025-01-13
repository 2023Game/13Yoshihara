//�v���C���[�N���X�̃C���N���[�h
#include "CPlayerBase.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CInteractObject.h"
#include "CSceneManager.h"
#include "CNavNode.h"
#include "CNavManager.h"

// �v���C���[�̃C���X�^���X
CPlayerBase* CPlayerBase::spInstance = nullptr;



#define GRAVITY 0.0625f

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

// �R���X�g���N�^
CPlayerBase::CPlayerBase()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsWall(false)
	, mpRideObject(nullptr)
	, mMotionBlurRemainTime(0.0f)
	, mIsDamage(false)
	, mpBodyCol(nullptr)
	, mpAttackCol(nullptr)
	, mpSearchCol(nullptr)
{
	spInstance = this;

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);
}

// �f�X�g���N�^
CPlayerBase::~CPlayerBase()
{
	// �R���C�_�\���폜
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpAttackCol);
	SAFE_DELETE(mpSearchCol);

	// �o�H�T���p�̃m�[�h���폜
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
	}

	// �C���X�^���X�ƍ폜���Ă���v���C���[������Ȃ�폜
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

CPlayerBase* CPlayerBase::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CPlayerBase::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

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

	//mMoveSpeedY -= GRAVITY;
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

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("Wall:%s\n", mIsWall ? "true" : "false");
	CDebugPrint::Print("Pos:%f,%f,%f\n", Position().X(), Position().Y(), Position().Z());

	mIsGrounded = false;
	mIsWall = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());

	// ���ׂ�I�u�W�F�N�g�̃��X�g���N���A
	mNearInteractObjs.clear();
}

/*
�Փˏ���
�{�̃R���C�_�ƒn�`�̏Փˏ���
*/
void CPlayerBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̃R���C�_�Ƃ̏Փ˔���
	if (self == mpBodyCol)
	{
		// �Փ˂������肪�t�B�[���h�̏ꍇ
		if (other->Layer() == ELayer::eGround)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);

			// �Փ˂����n�ʂ������V�䂩����ςŔ���
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// ���ς̌��ʂ��v���X�ł���΁A���ƏՓ˂���
			if (dot >= 0.0f)
			{
				// �����Ȃǂŏ��ɏォ��Փ˂����Ƃ��i���ړ��j�̂�
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
			}
			// ���ς̌��ʂ��}�C�i�X�ł���΁A�V��ƏՓ˂���
			else if (dot < 0.0f)
			{
				// �W�����v�ȂǂœV��ɂ�������Փ˂����Ƃ��i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// �Փ˂������肪�ǂ̏ꍇ
		else if (other->Layer() == ELayer::eWall)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
		// �Փ˂������肪�I�u�W�F�N�g�������ꍇ
		else if (other->Layer() == ELayer::eObject)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
	// ���ׂ�I�u�W�F�N�g�̒T�m�R���C�_�[�̏Փ˔���
	else if (self == mpSearchCol)
	{
		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr)
		{
			// �Փ˂������ׂ�I�u�W�F�N�g�����X�g�ɒǉ�
			mNearInteractObjs.push_back(obj);

#if _DEBUG
			// �T�m�͈͓��ɓ������I�u�W�F�N�g�̖��O��\��
			CDebugPrint::Print
			(
				"%s:%s\n",
				obj->GetDebugName().c_str(),
				obj->GetInteractStr().c_str()
			);
#endif
		}
	}
}

// �`��
void CPlayerBase::Render()
{
	CXCharacter::Render();
}

// ��ԋ߂��ɂ��钲�ׂ�I�u�W�F�N�g���擾
CInteractObject* CPlayerBase::GetNearInteractObject() const
{
	// ��ԋ߂��̒��ׂ�I�u�W�F�N�g�̃|�C���^�i�[�p
	CInteractObject* nearObj = nullptr;
	float nearDist = 0.0f;	// ���݈�ԋ߂��ɂ��钲�ׂ�I�u�W�F�N�g�Ƃ̋���
	CVector pos = Position();
	// �T�m�͈͓��̒��ׂ�I�u�W�F�N�g�����Ԃɒ��ׂ�
	for (CInteractObject * obj : mNearInteractObjs)
	{
		// ���ݒ��ׂ����Ԃ���Ȃ���΃X���[
		if (!obj->CanInteract()) continue;

		float dist = (obj->Position() - pos).LengthSqr();
		// ��ԍŏ��̒��ׂ�I�u�W�F�N�g��
		// ���߂����������݂̈�ԋ߂��I�u�W�F�N�g�����߂��ꍇ
		if (nearObj == nullptr || dist < nearDist)
		{
			// ��ԋ߂��I�u�W�F�N�g���X�V
			nearObj = obj;
			nearDist = dist;
		}
	}
	return nearObj;
}