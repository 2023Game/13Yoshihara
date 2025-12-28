#include "CSwitchMoveFloor.h"
#include "CColliderMesh.h"
#include "CConnectPointManager.h"
#include <typeinfo>

// 一時停止の時間
constexpr float STOP_TIME = 0.5f;

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct SwitchMoveFloorSaveData {
	CVector pos;		// 現在座標
	float elapsedTime;		// 経過時間
	CSwitchObject::EState state;	// 状態
	EMoveState moveState;			// 移動状態
	EMoveState preMoveState;		// 前の移動状態
};
#pragma pack(pop)	// 設定を元に戻す

std::vector<char> CSwitchMoveFloor::SaveState() const
{
	SwitchMoveFloorSaveData data;
	data.pos = Position();
	data.elapsedTime = GetElapsedTime();
	data.state = GetState();
	data.moveState = GetMoveState();
	data.preMoveState = GetPreMoveState();

	// データをバイト列に変換して返す
	const char* dataPtr = reinterpret_cast<const char*>(&data);
	return std::vector<char>(dataPtr,dataPtr + sizeof(data));
}

void CSwitchMoveFloor::LoadState(const std::vector<char>& data)
{
	if (data.size() != sizeof(SwitchMoveFloorSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const SwitchMoveFloorSaveData* saveData = reinterpret_cast<const SwitchMoveFloorSaveData*>(data.data());
	Position(saveData->pos);
	SetElapsedTime(saveData->elapsedTime);
	SetMoveState(saveData->moveState);
	SetPreMoveState(saveData->preMoveState);
}

size_t CSwitchMoveFloor::GetTypeID() const
{
	return typeid(CSwitchMoveFloor).hash_code();
}

unsigned int CSwitchMoveFloor::GetUniqueInstanceID() const
{
	return mUniqueID;
}

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
	, mUniqueID(CUIDManager::GenerateNewID())
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

void CSwitchMoveFloor::SetMoveState(EMoveState moveState)
{
	if (mMoveState != moveState)
	{
		mMoveState = moveState;
	}
}

EMoveState CSwitchMoveFloor::GetMoveState() const
{
	return mMoveState;
}

void CSwitchMoveFloor::SetPreMoveState(EMoveState moveState)
{
	if (mPreMoveState != moveState)
	{
		mPreMoveState = moveState;
	}
}

EMoveState CSwitchMoveFloor::GetPreMoveState() const
{
	return mPreMoveState;
}

// コライダーを生成
void CSwitchMoveFloor::CreateCol()
{
	// 本体のコライダー
	mpCol = new CColliderMesh(this, ELayer::eField, mpModel, true);

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