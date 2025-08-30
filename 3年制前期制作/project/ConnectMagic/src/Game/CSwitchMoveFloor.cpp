#include "CSwitchMoveFloor.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"

// 一時停止の時間
#define STOP_TIME 0.5f

// コンストラクタ
CSwitchMoveFloor::CSwitchMoveFloor(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime, bool isCrushed)
	: CSwitchObject()
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mMoveState(EMoveState::eStop)
	, mPreMoveState(EMoveState::eBack)
	, mpCrushedCol(nullptr)
{
	mpModel = model;

	// タグを乗ることが出来るオブジェクトに設定
	SetTag(ETag::eRideableObject);

	// コライダーを生成
	CreateCol();
	// プレイヤーを壊すなら
	if (isCrushed)
	{
		// プレイヤーが挟まれた時用のコライダー
		mpCrushedCol = new CColliderMesh(this, ELayer::eCrushed, col, true);
		// プレイヤーとだけ衝突
		mpCrushedCol->SetCollisionLayers({ ELayer::ePlayer });
	}

	// 初期座標の設定
	Position(mDefaultPos);
	Scale(scale);
}

// デストラクタ
CSwitchMoveFloor::~CSwitchMoveFloor()
{
	SAFE_DELETE(mpCrushedCol);
}

// コライダーを生成
void CSwitchMoveFloor::CreateCol()
{
	// 本体のコライダー
	mpCol = new CColliderMesh(this, ELayer::eGround, mpModel, true);

	// 接続部の管理クラス
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// コライダーを追加
	pointMgr->AddCollider(mpCol);
	// 現在のカメラ
	CCamera* camera = CCamera::CurrentCamera();
	// コライダーを追加
	camera->AddCollider(mpCol);
}

// 作用している時の処理
void CSwitchMoveFloor::UpdateOn()
{
	mElapsedTime += Times::DeltaTime();

	switch (mMoveState)
	{
	case EMoveState::eStop:
		UpdateStop();
		break;
	case EMoveState::eGo:
		UpdateGo();
		break;
	case EMoveState::eBack:
		UpdateBack();
		break;
	}
}

// 状態を変更
void CSwitchMoveFloor::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;

	// 一つ前の状態を更新
	mPreMoveState = mMoveState;
	// 状態を変更
	mMoveState = state;
	mElapsedTime = 0.0f;
}

// 止まっているときの更新
void CSwitchMoveFloor::UpdateStop()
{
	// 一時停止の時間が経過したら
	if (mElapsedTime >= STOP_TIME)
	{
		// 一つ前が進む状態だったら
		if (mPreMoveState == EMoveState::eGo)
		{
			// 戻る状態に変更
			ChangeMoveState(EMoveState::eBack);
		}
		// 戻る状態なら
		else if (mPreMoveState == EMoveState::eBack)
		{
			// 進む状態へ
			ChangeMoveState(EMoveState::eGo);
		}
	}
}

// 進んでいるときの更新
void CSwitchMoveFloor::UpdateGo()
{
	float per = mElapsedTime / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// 時間が経過したら一時停止状態へ
	if (mElapsedTime >= mMoveTime)
	{
		ChangeMoveState(EMoveState::eStop);
	}
}

// 戻っているときの更新
void CSwitchMoveFloor::UpdateBack()
{
	float per = (mMoveTime - mElapsedTime) / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// 時間が経過したら一時停止状態へ
	if (mElapsedTime >= mMoveTime)
	{
		ChangeMoveState(EMoveState::eStop);
	}
}

// 状態を設定
void CSwitchMoveFloor::SetState(EMoveState state)
{
	mMoveState = state;
}

// 状態を取得
EMoveState CSwitchMoveFloor::GetState() const
{
	return mMoveState;
}

// 前回の状態を設定
void CSwitchMoveFloor::SetPreState(EMoveState state)
{
	mPreMoveState = state;
}

// 前回の状態を取得
EMoveState CSwitchMoveFloor::GetPreState() const
{
	return mPreMoveState;
}

// 経過時間を設定
void CSwitchMoveFloor::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// 経過時間を取得
float CSwitchMoveFloor::GetElapsedTime() const
{
	return mElapsedTime;
}