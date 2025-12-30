#include "CMoveObj.h"
#include "Maths.h"
#include "CConnectPointManager.h"
#include "CPhysicsManager.h"
#include "PhysicsMaterial.h"
#include <typeinfo>

// 一時停止の時間
constexpr float STOP_TIME = 0.5f;

constexpr float MASS = 0.0f;
const CVector HALF_EXTENTS = CVector(5.0f, 5.0f, 5.0f);
const CVector SENSOR_HALF_EXTENTS = CVector(4.0f, 4.0f, 4.0f);

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct MoveObjSaveData {
	CVector pos;		// 現在座標
	float elapsedTime;	// 経過時間
	EMoveState moveState;	// 移動状態
	EMoveState preMoveState;// 前の移動状態
};
#pragma pack(pop)	// 設定を元に戻す

std::vector<char> CMoveObj::SaveState() const
{
	MoveObjSaveData data;
	data.pos = Position();				// 現在座標
	data.elapsedTime = GetElapsedTime();// 経過時間
	data.moveState = GetMoveState();			// 状態
	data.preMoveState = GetPreMoveState();		// 前の状態

	// データをバイト列に変換して返す
	const char* dataPtr = reinterpret_cast<const char*>(&data);
	return std::vector<char>(dataPtr, dataPtr + sizeof(data));
}

void CMoveObj::LoadState(const std::vector<char>& data)
{
	// データのサイズチェック
	if (data.size() != sizeof(MoveObjSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const MoveObjSaveData* saveData = reinterpret_cast<const MoveObjSaveData*>(data.data());
	Position(saveData->pos);
	SetElapsedTime(saveData->elapsedTime);
	SetMoveState(saveData->moveState);
	SetPreMoveState(saveData->preMoveState);
}

size_t CMoveObj::GetTypeID() const
{
	return typeid(CMoveObj).hash_code();
}

unsigned int CMoveObj::GetUniqueInstanceID() const
{
	return mUniqueID;
}

CMoveObj::CMoveObj(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale,
	const CVector& move, float moveTime,
	ELayer layer, bool isCrushed)
	: mpModel(model)
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mMoveState(EMoveState::eStop)
	, mPreMoveState(EMoveState::eBack)
	, mUniqueID(CUIDManager::GenerateNewID())
{
	Position(mDefaultPos);
	Scale(scale);

	// コライダーを生成
	CreateCol();
	// プレイヤーを壊すなら
	if (isCrushed)
	{
		CPhysicsManager::Instance()->CreateBoxSensor(
			this,
			SENSOR_HALF_EXTENTS,
			ELayer::eCrushed,
			{ ELayer::ePlayer }
		);
	}
}

CMoveObj::~CMoveObj()
{
}

void CMoveObj::Update()
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

void CMoveObj::Render()
{
	mpModel->Render(Matrix());
}

void CMoveObj::CreateCol()
{
	PhysicsMaterial material;
	material.mass = MASS;

	CPhysicsManager::Instance()->CreateBoxRigidBody(
		this,
		material,
		HALF_EXTENTS,
		Position(),
		Rotation(),
		ELayer::eField,
		{ ELayer::ePlayer,ELayer::eObject,ELayer::eConnectObj }
	);
}

// 状態を変更
void CMoveObj::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;
	
	// 一つ前の状態を更新
	mPreMoveState = mMoveState;
	// 状態を変更
	mMoveState = state;
	mElapsedTime = 0.0f;
}

// 状態を設定
void CMoveObj::SetMoveState(EMoveState state)
{
	mMoveState = state;
}

// 状態を取得
EMoveState CMoveObj::GetMoveState() const
{
	return mMoveState;
}

// 前回の状態を設定
void CMoveObj::SetPreMoveState(EMoveState state)
{
	mPreMoveState = state;
}

// 前回の状態を取得
EMoveState CMoveObj::GetPreMoveState() const
{
	return mPreMoveState;
}

// 経過時間を設定
void CMoveObj::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// 経過時間を取得
float CMoveObj::GetElapsedTime() const
{
	return mElapsedTime;
}

// 止まっているときの更新
void CMoveObj::UpdateStop()
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
void CMoveObj::UpdateGo()
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
void CMoveObj::UpdateBack()
{
	float per = (mMoveTime - mElapsedTime) / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// 時間が経過したら一時停止状態へ
	if (mElapsedTime >= mMoveTime)
	{
		ChangeMoveState(EMoveState::eStop);
	}
}
