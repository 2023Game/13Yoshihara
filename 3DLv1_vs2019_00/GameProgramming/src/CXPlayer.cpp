#include "CXPlayer.h"	

//�R���C�_�̏�����
CXPlayer::CXPlayer()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
	//�^�O�Ƀv���C���[��ݒ肷��
	mTag = ETag::EPLAYER;
}

void CXPlayer::Update()
{
	if (AnimationIndex() == 3)
	{
		if (IsAnimationFinished())
		{
			ChangeAnimation(4, false, 30);
		}
	}
	if (AnimationIndex() == 4)
	{
		if (IsAnimationFinished())
		{
			ChangeAnimation(0, true, 60);
		}
	}
	if (AnimationIndex() != 3 && AnimationIndex() != 4)
	{
		//�O�ړ�
		if (mInput.Key('W'))
		{
			ChangeAnimation(1, true, 60);
			mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
		}
		//�ҋ@
		else
		{
			ChangeAnimation(0, true, 60);
		}
		//���ړ�
		if (mInput.Key('A'))
		{
			mRotation += CVector(0.0f, 2.0f, 0.0f);
		}
		//�E�ړ�
		if (mInput.Key('D'))
		{
			mRotation += CVector(0.0f, -2.0f, 0.0f);
		}

		//�U��
		if (mInput.Key(VK_SPACE))
		{
			ChangeAnimation(3, false, 30);
		}
	}
	

	CXCharacter::Update();
}


void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.Matrix(&mpCombinedMatrix[12]);
	//��
	mColSphereBody.Matrix(&mpCombinedMatrix[9]);
	//��
	mColSphereSword.Matrix(&mpCombinedMatrix[22]);
}
