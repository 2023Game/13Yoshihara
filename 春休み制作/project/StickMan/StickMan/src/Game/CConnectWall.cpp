#include "CConnectWall.h"

#include "CInput.h"

// 開いたときの回転値
#define OPEN_ROT 90.0f

// 開閉の速度
#define ROT_SPEED 180.0f

// コンストラクタ
CConnectWall::CConnectWall()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mIsOpen(false)
	, mDefaultRot(CVector::zero)
	, mTotalRot(CVector::zero)
{
	mpModel = CResourceManager::Get<CModel>("Book");

	// コライダ―を生成
	//CreateCol();
}

// デストラクタ
CConnectWall::~CConnectWall()
{
}

// 状態切り替え
void CConnectWall::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// コライダ―を生成
void CConnectWall::CreateCol()
{
	mpMeshCol = new CColliderMesh
	{
		this, ELayer::eWall,
		mpModel,
		true
	};
}

// 開閉する
void CConnectWall::OpenClose()
{		
	// 開いているなら
	if (mIsOpen)
	{
		// 閉じる状態へ
		ChangeState(EState::eClose);
	}
	// 閉じているなら
	else
	{
		// 開く状態へ
		ChangeState(EState::eOpen);
	}
}

// 更新
void CConnectWall::Update()
{
	if (CInput::PushKey('O'))
	{
		// 開閉する
		OpenClose();
	}

	switch (mState)
	{
	case EState::eIdle:					break;
	case EState::eOpen:		Open();		break;
	case EState::eClose:	Close();	break;
	}
}

// 描画
void CConnectWall::Render()
{
	mpModel->Render(Matrix());
}

// 初期回転値を設定
void CConnectWall::SetDefaultRot(CVector rot)
{
	mDefaultRot = rot;
	// 回転を設定
	Rotation(rot);
}

// 開く
void CConnectWall::Open()
{
	switch (mStateStep)
	{
	case 0:
	{
		// 時間経過
		mElapsedTime += Times::DeltaTime();

		// 回転量
		CVector rot = CVector(ROT_SPEED * mElapsedTime, 0.0f, 0.0f);
		// 総回転量に加算
		mTotalRot += rot;

		// 回転が終了したら
		if (mTotalRot.X() >= OPEN_ROT)
		{
			// 開いた状態の回転に設定
			Rotation(mDefaultRot + CVector(OPEN_ROT, 0.0f, 0.0f));
			
			// 総回転量をリセット
			mTotalRot = CVector::zero;
			// 次へ
			mStateStep++;
		}
		// 回転が終了していないなら
		else
		{
			// 回転量を適用
			Rotate(rot);
			// 経過時間をリセット
			mElapsedTime = 0.0f;
		}
		break;
	}
	case 1:
		// 開いている
		mIsOpen = true;
		// 待機状態へ
		ChangeState(EState::eIdle);
		break;
	}
}

// 閉じる
void CConnectWall::Close()
{		
	switch (mStateStep)
	{
	case 0:
	{
		// 時間経過
		mElapsedTime += Times::DeltaTime();

		// 回転量
		CVector rot = CVector(ROT_SPEED * mElapsedTime, 0.0f, 0.0f);
		// 総回転量に加算
		mTotalRot += rot;

		// 回転が終了したら
		if (mTotalRot.X() >= OPEN_ROT)
		{
			// 初期回転に戻す
			Rotation(mDefaultRot);

			// 総回転量をリセット
			mTotalRot = CVector::zero;
			// 次へ
			mStateStep++;
		}
		// 回転が終了していないなら
		else
		{
			// 回転量を適用
			Rotate(-rot);
			// 経過時間をリセット
			mElapsedTime = 0.0f;
		}
		break;
	}
	case 1:
		// 閉じている
		mIsOpen = false;
		// 待機状態へ
		ChangeState(EState::eIdle);
		break;
	}
}
