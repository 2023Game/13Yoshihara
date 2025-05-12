#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "CModel.h"

#define THRESHOLD 0.1f

// �R���X�g���N�^
CConnectObject::CConnectObject(float weight, ETaskPriority prio,
	int sortOrder,ETaskPauseType pause)
	: CObjectBase(ETag::eConnectObject, prio, 0, pause)
	, mWeight(weight)
	, mpCol(nullptr)
	, mpModel(nullptr)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGravity(true)
	, mConnectObjTag(EConnectObjTag::eBox)
{
}

// �f�X�g���N�^
CConnectObject::~CConnectObject()
{
}

// �I�u�W�F�N�g�폜��`����֐�
void CConnectObject::DeleteObject(CObjectBase* obj)
{
}

// �X�V
void CConnectObject::Update()
{
	if (mIsGravity)
	{
		mMoveSpeedY -= GRAVITY;
	}
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	if (mMoveSpeed.LengthSqr() > 0.0f)
	{
		CDebugPrint::Print("ObjMoveSpeed:%f,%f,%f\n", moveSpeed.X(), moveSpeed.Y(), moveSpeed.Z());
	}
	mMoveSpeed = CVector::zero;

	// �ړ�
	Position(Position() + moveSpeed);
}

// �`��
void CConnectObject::Render()
{
	mpModel->Render(Matrix());
}

// �Փˏ���
void CConnectObject::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpCol)
	{
		// �Փ˂������肪�n�ʂ̏ꍇ
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
			}
			// ���ς̌��ʂ��}�C�i�X�ł���΁A�V��ƏՓ˂���
			else if (dot < 0.0f)
			{
				// �V��ɂ�������Փ˂����Ƃ��i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// �Փ˂������肪�ǁA�I�u�W�F�N�g�A�v���C���[��
		// �����ꂩ�̏ꍇ
		else if (other->Layer() == ELayer::eWall)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
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

// ��������ꂽ���̏���
void CConnectObject::Pull(CVector pullDir, float opponentWeight)
{
	// ���g�������Ȃ��d���̏ꍇ�������Ȃ�
	if (mWeight == 1.0f) return;
	// �ړ����x
	CVector moveSpeed = CVector::zero;
	// ���肪�����Ȃ��d���Ȃ�
	if (opponentWeight == 1.0f)
	{
		// ��������͂͂��̂܂�
		moveSpeed = pullDir * PULL_POWER * Times::DeltaTime();
	}
	// ����ȊO�Ȃ�
	else
	{
		// �����Ƒ���̏d���̑���
		float totalWeight = mWeight + opponentWeight;
		// ���ʂɑ΂��鑊��̏d���ň�������͂����߂�
		moveSpeed = pullDir * (opponentWeight / totalWeight * PULL_POWER) * Times::DeltaTime();
	}

	// �ړ����x��ݒ肷��
	mMoveSpeedY = moveSpeed.Y();
	moveSpeed.Y(0.0f);
	mMoveSpeed = moveSpeed;
}

// �ڑ��^�[�Q�b�g�̍쐬
void CConnectObject::CreateTarget(CVector pos)
{
	// �^�[�Q�b�g����
	CConnectTarget* target = new CConnectTarget(this);
	// �e�q�ݒ�
	target->SetParent(this);
	// �ʒu�ݒ�
	target->Position(pos);
	// ���X�g�ɒǉ�
	mTargets.push_back(target);
}

// �ڑ��^�[�Q�b�g���擾
std::vector<CConnectTarget*> CConnectObject::GetTargets()
{
	return mTargets;
}

// �d�����擾
float CConnectObject::GetWeight()
{
	return mWeight;
}

// �d�͂��|���邩��ݒ�
void CConnectObject::SetGravity(bool isGravity)
{
	mIsGravity = isGravity;
}

// �ڑ��I�u�W�F�N�g�̃^�O��ݒ�
void CConnectObject::SetConnectObjTag(EConnectObjTag tag)
{
	mConnectObjTag = tag;
}

// �ڑ��I�u�W�F�N�g�̃^�O���擾
EConnectObjTag CConnectObject::GetConnectObjTag()
{
	return mConnectObjTag;
}
