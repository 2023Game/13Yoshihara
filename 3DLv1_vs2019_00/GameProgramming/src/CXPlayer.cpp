#include "CXPlayer.h"	

void CXPlayer::Update()
{
	//‘OˆÚ“®
	if (mInput.Key('W'))
	{
		ChangeAnimation(1, true, 60);
		mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
	}
	else
	{
		ChangeAnimation(0, true, 60);
	}
	if (mInput.Key('A'))
	{
		mRotation += CVector(0.0f, 2.0f, 0.0f);
	}
	if (mInput.Key('D'))
	{
		mRotation += CVector(0.0f, -2.0f, 0.0f);
	}

	CXCharacter::Update();
}
