#include "CEnemyBase.h"
#include "CDebugFieldOfView.h"

#define GRAVITY 0.0625f

// �R���X�g���N�^
CEnemyBase::CEnemyBase(float fovAngle, float fovLength)
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsWall(false)
	, mpRideObject(nullptr)
	, mIsDamage(false)
	, mpBodyCol(nullptr)
	, mFovAngle(fovAngle)
	, mFovLength(fovLength)
	, mpDebugFov(nullptr)
{
	// ����͈͂̃f�o�b�O�\���N���X���쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
}

// �f�X�g���N�^
CEnemyBase::~CEnemyBase()
{
	// �R���C�_�\�폜
	SAFE_DELETE(mpBodyCol);
	// ����͈͂̃f�o�b�O�\�������݂�����A�ꏏ�ɍ폜
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}
}

// �I�u�W�F�N�g�폜����
void CEnemyBase::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�I�u�W�F�N�g������͈͂̃f�o�b�O�\���ł����
	// �|�C���^����ɂ���
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

// �X�V
void CEnemyBase::Update()
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
		// �G���ړ������֌�����
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;
	mIsWall = false;
}

// �`��
void CEnemyBase::Render()
{
	CXCharacter::Render();
}

/*
�Փˏ���
�{�̃R���C�_�ƒn�`�̏Փ�
*/
void CEnemyBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̃R���C�_�Ƃ̏Փ˔���
	if (self == mpBodyCol)
	{
		// �Փ˂������肪�t�B�[���h�̏ꍇ
		if (other->Layer() == ELayer::eField)
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
}

// �A�j���[�V�����؂�ւ�
void CEnemyBase::ChangeAnimation(int type, bool restart)
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
