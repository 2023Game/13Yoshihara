#include "CConnectWall.h"

#include "CInput.h"

// �J�����Ƃ��̉�]�l
#define OPEN_ROT 90.0f

// �J�̑��x
#define ROT_SPEED 180.0f

// �R���X�g���N�^
CConnectWall::CConnectWall()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mIsOpen(false)
	, mDefaultRot(CVector::zero)
	, mTotalRot(CVector::zero)
{
	mpModel = CResourceManager::Get<CModel>("Book");

	// �R���C�_�\�𐶐�
	//CreateCol();
}

// �f�X�g���N�^
CConnectWall::~CConnectWall()
{
}

// ��Ԑ؂�ւ�
void CConnectWall::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// �R���C�_�\�𐶐�
void CConnectWall::CreateCol()
{
	mpMeshCol = new CColliderMesh
	{
		this, ELayer::eWall,
		mpModel,
		true
	};
}

// �J����
void CConnectWall::OpenClose()
{		
	// �J���Ă���Ȃ�
	if (mIsOpen)
	{
		// �����Ԃ�
		ChangeState(EState::eClose);
	}
	// ���Ă���Ȃ�
	else
	{
		// �J����Ԃ�
		ChangeState(EState::eOpen);
	}
}

// �X�V
void CConnectWall::Update()
{
	if (CInput::PushKey('O'))
	{
		// �J����
		OpenClose();
	}

	switch (mState)
	{
	case EState::eIdle:					break;
	case EState::eOpen:		Open();		break;
	case EState::eClose:	Close();	break;
	}
}

// �`��
void CConnectWall::Render()
{
	mpModel->Render(Matrix());
}

// ������]�l��ݒ�
void CConnectWall::SetDefaultRot(CVector rot)
{
	mDefaultRot = rot;
	// ��]��ݒ�
	Rotation(rot);
}

// �J��
void CConnectWall::Open()
{
	switch (mStateStep)
	{
	case 0:
	{
		// ���Ԍo��
		mElapsedTime += Times::DeltaTime();

		// ��]��
		CVector rot = CVector(ROT_SPEED * mElapsedTime, 0.0f, 0.0f);
		// ����]�ʂɉ��Z
		mTotalRot += rot;

		// ��]���I��������
		if (mTotalRot.X() >= OPEN_ROT)
		{
			// �J������Ԃ̉�]�ɐݒ�
			Rotation(mDefaultRot + CVector(OPEN_ROT, 0.0f, 0.0f));
			
			// ����]�ʂ����Z�b�g
			mTotalRot = CVector::zero;
			// ����
			mStateStep++;
		}
		// ��]���I�����Ă��Ȃ��Ȃ�
		else
		{
			// ��]�ʂ�K�p
			Rotate(rot);
			// �o�ߎ��Ԃ����Z�b�g
			mElapsedTime = 0.0f;
		}
		break;
	}
	case 1:
		// �J���Ă���
		mIsOpen = true;
		// �ҋ@��Ԃ�
		ChangeState(EState::eIdle);
		break;
	}
}

// ����
void CConnectWall::Close()
{		
	switch (mStateStep)
	{
	case 0:
	{
		// ���Ԍo��
		mElapsedTime += Times::DeltaTime();

		// ��]��
		CVector rot = CVector(ROT_SPEED * mElapsedTime, 0.0f, 0.0f);
		// ����]�ʂɉ��Z
		mTotalRot += rot;

		// ��]���I��������
		if (mTotalRot.X() >= OPEN_ROT)
		{
			// ������]�ɖ߂�
			Rotation(mDefaultRot);

			// ����]�ʂ����Z�b�g
			mTotalRot = CVector::zero;
			// ����
			mStateStep++;
		}
		// ��]���I�����Ă��Ȃ��Ȃ�
		else
		{
			// ��]�ʂ�K�p
			Rotate(-rot);
			// �o�ߎ��Ԃ����Z�b�g
			mElapsedTime = 0.0f;
		}
		break;
	}
	case 1:
		// ���Ă���
		mIsOpen = false;
		// �ҋ@��Ԃ�
		ChangeState(EState::eIdle);
		break;
	}
}
