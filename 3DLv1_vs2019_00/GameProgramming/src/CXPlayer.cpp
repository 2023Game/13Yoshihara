#include "CXPlayer.h"	

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
