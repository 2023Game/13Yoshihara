//�v���C���[�N���X�̃C���N���[�h
#include "CPlayerBase.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "Maths.h"
#include "CSceneManager.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CGaugeUI2D.h"

// �v���C���[�̃C���X�^���X
CPlayerBase* CPlayerBase::spInstance = nullptr;

#define THRESHOLD 0.1f

// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_TIME 3.0f
// ���[�V�����u���[�̕�
#define MOTION_BLUR_WIDTH 1.0f
// ���[�V�����u���[�̔�����
#define MOTION_BLUR_COUNT 5

// �R���X�g���N�^
CPlayerBase::CPlayerBase()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mMotionBlurRemainTime(0.0f)
	, mpBodyCol(nullptr)
	, mpAttackCol(nullptr)
	, mpHpGauge(nullptr)
	, mIsGameEnd(false)
{
	mpTestModel = CResourceManager::Get<CModel>("TestModel");
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

	// HP�Q�[�W�����݂�����A�ꏏ�ɍ폜����
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
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

// �A�j���[�V�����u�����h�؂�ւ�
bool CPlayerBase::ChangeAnimationBlend(int type, float blendSpeed, bool restart)
{
	bool end = false;
	if (mpAnimData == nullptr) return end;
	if (!(0 <= type && type < mpAnimData->size())) return end;
	AnimData data = (*mpAnimData)[type];
	end = CXCharacter::ChangeAnimationBlend
	(
		type,
		data.loop,
		data.frameLength,
		restart,
		blendSpeed
	);
	CXCharacter::SetAnimationSpeed(data.speed);
	return end;
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

// �I�u�W�F�N�g�폜����
void CPlayerBase::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�I�u�W�F�N�g��Hp�Q�[�W�ł����
	// �|�C���^����ɂ���
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
}

// �X�V
void CPlayerBase::Update()
{
	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}


#if _DEBUG
	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("Pos:%f,%f,%f\n", Position().X(), Position().Y(), Position().Z());
	CDebugPrint::ForcePrint("FPS:%f\n", Times::FPS());
#endif

	mIsGrounded = false;
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

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);

			// �Փ˂����ʂ��ォ��������ςŔ���
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// ���ς̌��ʂ��v���X�ł���΁A��ʂƏՓ˂���
			if (dot >= THRESHOLD)
			{
				// �����Ȃǂŏォ��Փ˂����Ƃ��i���ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// �ڒn����
				mIsGrounded = true;
				// �ڒn�����ʂ̖@�����L�����Ă���
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// ���ς̌��ʂ��}�C�i�X�ł���΁A���ʂƏՓ˂���
			else if (dot < 0.0f)
			{
				// �W�����v�Ȃǂŉ�����Փ˂����Ƃ��i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
	}
}

// �`��
void CPlayerBase::Render()
{
	CXCharacter::Render();
	mpTestModel->Render(Matrix());
}

// ���S�ɂ���ăQ�[�����I�����邩���擾����
bool CPlayerBase::GetGameEnd()
{
	return mIsGameEnd;
}

// ���S�ɂ���ăQ�[�����I�����邩��ݒ肷��
void CPlayerBase::SetGameEnd(bool isEnd)
{
	mIsGameEnd = isEnd;
}
