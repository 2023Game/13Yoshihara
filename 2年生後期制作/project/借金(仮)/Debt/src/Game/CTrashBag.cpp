#include "CTrashBag.h"
#include "CModel.h"
#include "CColliderSphere.h"

#include "CTrashPlayer.h"
#include "CTrashEnemy.h"

// ��{�̏d��
#define GRAVITY 0.0625f

// �{�̃R���C�_�\
#define BODY_RADIUS 2.5f	// ���a
// �I�t�Z�b�g���W
#define BODY_COL_OFFSET_POS CVector(0.0f,1.2f,0.0f)

#define SCALE 2.0f

// �R���X�g���N�^
CTrashBag::CTrashBag(bool gold)
	: CObjectBase(ETag::eTrashBag, ETaskPriority::eTrashBag)
	, CTrashBagStatus(gold)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mpRideObject(nullptr)
	, mGroundNormal(CVector::zero)
	, mIsGrounded(false)
	, mIsGravity(true)
{
	Scale(SCALE, SCALE, SCALE);
	// �S�[���h�̏ꍇ
	if (gold)
	{
		// �S�[���h�̃��f��
		mpModel = CResourceManager::Get<CModel>("TrashBag");
	}
	// �ʏ�̏ꍇ
	else
	{
		// �ʏ�̃��f��
		mpModel = CResourceManager::Get<CModel>("TrashBag");
	}

	// �{�̃R���C�_�\
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eTrashBag,
		BODY_RADIUS
	);
	mpBodyCol->Position(BODY_COL_OFFSET_POS);
	// �v���C���[�A�G�A������A�n�`�A�S�~�܁A�ԗ�
	// �ƏՓ˔��肷��
	mpBodyCol->SetCollisionTags({ ETag::ePlayer,ETag::eEnemy,ETag::eField,ETag::eTrashBag,ETag::eVehicle });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer,ELayer::eEnemy,ELayer::eCollector,
		ELayer::eGround,ELayer::eWall,ELayer::eObject,ELayer::eTrashBag,ELayer::eVehicle });
}

// �f�X�g���N�^
CTrashBag::~CTrashBag()
{
}

// �X�V
void CTrashBag::Update()
{
	// �d�͂��|����Ȃ�
	if (mIsGravity)
	{
		mMoveSpeedY -= GRAVITY;
	}

		CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

		// �ړ�
		Position(Position() + moveSpeed);

	// �n�ʂɂ��Ă���Ȃ�
	if (mIsGrounded)
	{
		// ���ł܂ł��J�E���g
		CountDeleteTime();
		// �ړ����[���ɂ���
		mMoveSpeed = CVector::zero;
	}

	// ���ł܂ł̎��Ԃ��o������
	if (IsElapsedDeleteTime())
	{
		SetOnOff(false);
	}

	mIsGrounded = false;

#if _DEBUG
	CDebugPrint::Print("TrashBagIsGold:%s\n", GetIsGold() ? "true" : "false");
#endif
}

// �Փˏ���
void CTrashBag::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̃R���C�_�\�̏ꍇ
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
		// �Փ˂������肪�v���C���[�Ȃ�
		else if (other->Layer() == ELayer::ePlayer)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);

			CTrashPlayer* player = dynamic_cast<CTrashPlayer*>(CPlayerBase::Instance());
			// �J���Ă���Ȃ�
			if (player->GetOpen())
			{
				// �����ɂ���
				SetOnOff(false);
			}
		}
		// �Փ˂������肪�G��������
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);

			CTrashEnemy* enemy = dynamic_cast<CTrashEnemy*>(other->Owner());
			// �J���Ă���Ȃ�
			if (enemy->GetOpen())
			{
				// �����ɂ���
				SetOnOff(false);
			}
		}
		// �Փ˂������肪�S�~�܂Ȃ�
		else if (other->Layer() == ELayer::eTrashBag)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
		// �Փ˂������肪�ԗ��Ȃ�
		else if (other->Layer() == ELayer::eVehicle)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
}

// �`��
void CTrashBag::Render()
{
	mpModel->Render(Matrix());
}

// �L�������ɂ���
void CTrashBag::SetOnOff(bool isOnOff)
{
	// �L��������ݒ�
	SetEnable(isOnOff);
	SetShow(isOnOff);

	// �L���ɂ���Ȃ���ł܂ł̎��Ԃ����Z�b�g
	if (isOnOff) SetDeleteTime();
}

// �S�[���h������
void CTrashBag::SetGold(bool gold)
{
	// �S�[���h�̏ꍇ
	if (gold)
	{
		// �S�[���h�̃��f��
		mpModel = CResourceManager::Get<CModel>("TrashBag");		
	}
	// �ʏ�̏ꍇ
	else
	{
		// �ʏ�̃��f��
		mpModel = CResourceManager::Get<CModel>("TrashBag");
	}
	SetIsGold(gold);
}

// ���ł������x��ݒ�
void CTrashBag::SetThrowSpeed(CVector speed, float speedY)
{
	mMoveSpeed = speed;
	mMoveSpeedY = speedY;
}

// �d�͂������邩��ݒ�
void CTrashBag::SetGravity(bool gravity)
{
	mIsGravity = gravity;
}
