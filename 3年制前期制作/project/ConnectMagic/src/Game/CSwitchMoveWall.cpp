#include "CSwitchMoveWall.h"
#include "CColliderMesh.h"
#include "Maths.h"
#include "CConnectPointManager.h"

// コンストラクタ
CSwitchMoveWall::CSwitchMoveWall(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime, bool isCrushed)
	: mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mpCrushedCol(nullptr)
	, mIsOpen(false)
{
	mpModel = model;

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

	// 初期座標を設定
	Position(mDefaultPos);
	Scale(scale);
}

// デストラクタ
CSwitchMoveWall::~CSwitchMoveWall()
{
	SAFE_DELETE(mpCrushedCol);
}

// 開いているか
bool CSwitchMoveWall::IsOpen() const
{
	return mIsOpen;
}

// 開いているかを設定
void CSwitchMoveWall::SetIsOpen(bool enable)
{
	mIsOpen = enable;
	// オンオフを切り替える
	SetOnOff(enable);
}

// コライダーを生成
void CSwitchMoveWall::CreateCol()
{
	mpCol = new CColliderMesh(this, ELayer::eWall, mpModel, true);

	// 接続部の管理クラス
	CConnectPointManager* pointMgr = CConnectPointManager::Instance();
	// コライダーを追加
	pointMgr->AddCollider(mpCol);
	// 現在のカメラ
	CCamera* camera = CCamera::CurrentCamera();
	// コライダーを追加
	camera->AddCollider(mpCol);
}

// 作用していない時の処理
void CSwitchMoveWall::UpdateOff()
{
	// 経過時間が0秒なら処理しない
	if (mElapsedTime == 0.0f) return;

	// 移動処理
	Move();

	// 経過時間を減少
	mElapsedTime -= Times::DeltaTime();
	// 経過時間が0以下なら
	if (mElapsedTime <= 0.0f)
	{
		// 開いていない
		mIsOpen = false;
		// 経過時間は0
		mElapsedTime = 0.0f;
		// 座標を更新
		Move();
	}
}

// 作用している時の処理
void CSwitchMoveWall::UpdateOn()
{
	// 移動時間が経過していたら処理しない
	if (mElapsedTime == mMoveTime) return;

	// 開いている
	mIsOpen = true;

	// 移動処理
	Move();

	// 時間経過
	mElapsedTime += Times::DeltaTime();
	// 移動時間が経過していたら
	if (mElapsedTime >= mMoveTime)
	{
		// 経過時間の値を移動時間にする
		mElapsedTime = mMoveTime;
		// 座標を更新
		Move();
	}
}

// 移動の処理
void CSwitchMoveWall::Move()
{
	// 時間が進んだ割合
	float per = mElapsedTime / mMoveTime;
	// 座標を求める
	Position(mDefaultPos + mMoveVec * per);
}

// 状態を変更
void CSwitchMoveWall::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;

	// 状態を変更
	mMoveState = state;
	mElapsedTime = 0.0f;
}

// 状態を設定
void CSwitchMoveWall::SetState(EMoveState state)
{
	mMoveState = state;
}

// 状態を取得
EMoveState CSwitchMoveWall::GetState() const
{
	return mMoveState;
}

// 経過時間を設定
void CSwitchMoveWall::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// 経過時間を取得
float CSwitchMoveWall::GetElapsedTime() const
{
	return mElapsedTime;
}