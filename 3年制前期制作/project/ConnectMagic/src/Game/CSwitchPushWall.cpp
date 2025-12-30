#include "CSwitchPushWall.h"
#include "CConnectPointManager.h"
#include "Maths.h"
#include "CPhysicsManager.h"
#include "PhysicsMaterial.h"
#include <typeinfo>

// 一時停止の時間
constexpr float STOP_TIME = 0.5f;

// 物理設定
constexpr float MASS = 0.0f;
const CVector HALF_EXTENTS = CVector(5.0f, 5.0f, 5.0f);
constexpr float FRICTION = 0.1f;	// 摩擦（値が高いと停止まで早くなる）
constexpr float LIN_DAMPING = 0.8f;	// 線形減衰(値が高いと滑りが小さくなる)
constexpr float ANG_DAMPING = 0.9f;	// 角減衰(値が高いと微細な回転振動を吸収する）

const CVector SENSOR_HALF_EXTENTS = CVector(5.0f, 10.0f, 5.0f);

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct SwitchPushWallSaveData {
	CVector pos;		// 現在座標
	float elapsedTime;	// 経過時間
	CSwitchObject::EState state;	// 状態
	EMoveState moveState;			// 移動状態
	EMoveState preMoveState;		// 前の移動状態
};
#pragma pack(pop)	// 設定を元に戻す

std::vector<char> CSwitchPushWall::SaveState() const
{
	SwitchPushWallSaveData data;
	data.pos = Position();
	data.elapsedTime = GetElapsedTime();
	data.state = GetState();
	data.moveState = GetMoveState();
	data.preMoveState = GetPreMoveState();

	return std::vector<char>();
}

void CSwitchPushWall::LoadState(const std::vector<char>& data)
{
	if (data.size() != sizeof(SwitchPushWallSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const SwitchPushWallSaveData* saveData = reinterpret_cast<const SwitchPushWallSaveData*>(data.data());
	Position(saveData->pos);
	SetElapsedTime(saveData->elapsedTime);
	SetState(saveData->state);
	SetMoveState(saveData->moveState);
	SetPreMoveState(saveData->preMoveState);
}

size_t CSwitchPushWall::GetTypeID() const
{
	return typeid(CSwitchPushWall).hash_code();
}

unsigned int CSwitchPushWall::GetUniqueInstanceID() const
{
	return mUniqueID;
}

void CSwitchPushWall::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

float CSwitchPushWall::GetElapsedTime() const
{
	return mElapsedTime;
}

CSwitchPushWall::CSwitchPushWall(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime, bool isCrushed)
	: CSwitchObject(ETaskPriority::eBackground)
	, mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mMoveState(EMoveState::eStop)
	, mPreMoveState(EMoveState::eBack)
	, mUniqueID(CUIDManager::GenerateNewID())
{
	mpModel = model;

	Position(mDefaultPos);
	Scale(scale);

	// コライダー生成
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

CSwitchPushWall::~CSwitchPushWall()
{
}

void CSwitchPushWall::SetMoveState(EMoveState moveState)
{
	if (mMoveState != moveState)
	{
		mMoveState = moveState;
	}
}

EMoveState CSwitchPushWall::GetMoveState() const
{
	return mMoveState;
}

void CSwitchPushWall::SetPreMoveState(EMoveState moveState)
{
	if (mPreMoveState != moveState)
	{
		mPreMoveState = moveState;
	}
}

EMoveState CSwitchPushWall::GetPreMoveState() const
{
	return mPreMoveState;
}

void CSwitchPushWall::CreateCol()
{
	// 物理設定
	PhysicsMaterial material;
	material.mass = MASS;
	material.friction = FRICTION;
	material.linDamping = LIN_DAMPING;
	material.angDamping = ANG_DAMPING;
	// サイズ計算
	CVector size = CVector(
		HALF_EXTENTS.X() * Scale().X(),
		HALF_EXTENTS.Y() * Scale().Y(),
		HALF_EXTENTS.Z() * Scale().Z());

	CPhysicsManager::Instance()->CreateBoxRigidBody(
		this,
		material,
		size,
		Position(),
		Rotation(),
		ELayer::eObject,
		{ ELayer::eField,ELayer::ePlayer,ELayer::eConnectObj }
	);
}

void CSwitchPushWall::UpdateOff()
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

// スイッチがオンになったら動かない
void CSwitchPushWall::UpdateOn()
{
}

// 止まっているときの更新
void CSwitchPushWall::UpdateStop()
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
void CSwitchPushWall::UpdateGo()
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
void CSwitchPushWall::UpdateBack()
{
	float per = (mMoveTime - mElapsedTime) / mMoveTime;
	Position(mDefaultPos + mMoveVec * per);

	// 時間が経過したら一時停止状態へ
	if (mElapsedTime >= mMoveTime)
	{
		ChangeMoveState(EMoveState::eStop);
	}
}

void CSwitchPushWall::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;

	// ひとつ前の状態を更新
	mPreMoveState = mMoveState;
	// 状態を変更
	mMoveState = state;
	mElapsedTime = 0.0f;
}
