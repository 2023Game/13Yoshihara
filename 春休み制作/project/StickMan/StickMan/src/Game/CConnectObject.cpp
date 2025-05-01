#include "CConnectObject.h"
#include "CConnectTarget.h"
#include "CModel.h"

// �R���X�g���N�^
CConnectObject::CConnectObject(float weight, ETaskPriority prio,
	int sortOrder,ETaskPauseType pause)
	: CObjectBase(ETag::eConnectObject, prio, 0, pause)
	, mWeight(weight)
	, mpCol(nullptr)
	, mpModel(nullptr)
	, mMoveSpeedY(0.0f)
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
				// �W�����v�ȂǂœV��ɂ�������Փ˂����Ƃ��i��ړ��j�̂�
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
	}
}

// ��������ꂽ���̏���
void CConnectObject::Pull()
{
}

// �ڑ��^�[�Q�b�g�̍쐬
void CConnectObject::CreateTarget(CVector pos)
{
	// �^�[�Q�b�g����
	CConnectTarget* target = new CConnectTarget();
	// �e�q�ݒ�
	target->SetParent(this);
	// �ʒu�ݒ�
	target->Position(pos);
	// ���X�g�ɒǉ�
	mTargets.push_back(target);
}
