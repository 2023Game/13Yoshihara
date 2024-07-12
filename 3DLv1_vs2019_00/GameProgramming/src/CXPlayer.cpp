#include "CXPlayer.h"	

//コライダの初期化
CXPlayer::CXPlayer()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
	//タグにプレイヤーを設定する
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
		//前移動
		if (mInput.Key('W'))
		{
			ChangeAnimation(1, true, 60);
			mPosition += CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
		}
		//待機
		else
		{
			ChangeAnimation(0, true, 60);
		}
		//左移動
		if (mInput.Key('A'))
		{
			mRotation += CVector(0.0f, 2.0f, 0.0f);
		}
		//右移動
		if (mInput.Key('D'))
		{
			mRotation += CVector(0.0f, -2.0f, 0.0f);
		}

		//攻撃
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
	//合成行列の設定
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[12]);
	//体
	mColSphereBody.Matrix(&mpCombinedMatrix[9]);
	//剣
	mColSphereSword.Matrix(&mpCombinedMatrix[22]);
}
