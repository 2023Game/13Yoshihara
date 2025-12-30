#include "CSwitchMoveWall.h"
#include "Maths.h"
#include "CConnectPointManager.h"
#include "CPhysicsManager.h"
#include "PhysicsMaterial.h"
#include <typeinfo>

// 物理設定
constexpr float MASS =			0.0f;
const CVector HALF_EXTENTS =	CVector(5.0f, 5.0f, 5.0f);
constexpr float FRICTION =		0.1f;	// 摩擦（値が高いと停止まで早くなる）
constexpr float LIN_DAMPING =	0.8f;	// 線形減衰(値が高いと滑りが小さくなる)
constexpr float ANG_DAMPING =	0.9f;	// 角減衰(値が高いと微細な回転振動を吸収する）

const CVector SENSOR_HALF_EXTENTS = CVector(5.0f, 10.0f, 5.0f);

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct SwitchMoveWallSaveData {
	CVector pos;		// 現在座標
	float elapsedTime;	// 経過時間
	CSwitchObject::EState state;	// 状態
	EMoveState moveState;			// 移動状態
};
#pragma pack(pop)	// 設定を元に戻す

std::vector<char> CSwitchMoveWall::SaveState() const
{
	SwitchMoveWallSaveData data;
	data.pos = Position();
	data.elapsedTime = GetElapsedTime();
	data.state = GetState();
	data.moveState = GetMoveState();

	// データをバイト列に変換して返す
	const char* dataPtr = reinterpret_cast<const char*>(&data);
	return std::vector<char>(dataPtr, dataPtr + sizeof(data));
}

void CSwitchMoveWall::LoadState(const std::vector<char>& data)
{
	if (data.size() != sizeof(SwitchMoveWallSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const SwitchMoveWallSaveData* saveData = reinterpret_cast<const SwitchMoveWallSaveData*>(data.data());
	Position(saveData->pos);
	SetElapsedTime(saveData->elapsedTime);
	SetState(saveData->state);
	SetMoveState(saveData->moveState);
}

size_t CSwitchMoveWall::GetTypeID() const
{
	return typeid(CSwitchMoveWall).hash_code();
}

unsigned int CSwitchMoveWall::GetUniqueInstanceID() const
{
	return mUniqueID;
}

// コンストラクタ
CSwitchMoveWall::CSwitchMoveWall(CModel* model, CModel* col,
	const CVector& pos, const CVector& scale, const CVector& move, float moveTime, bool isCrushed)
	: mDefaultPos(pos)
	, mMoveVec(move)
	, mMoveTime(moveTime)
	, mElapsedTime(0.0f)
	, mIsOpen(false)
	, mUniqueID(CUIDManager::GenerateNewID())
{
	mpModel = model;

	// 初期座標を設定
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

// デストラクタ
CSwitchMoveWall::~CSwitchMoveWall()
{
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

// 移動状態を設定
void CSwitchMoveWall::SetMoveState(EMoveState state)
{
	mMoveState = state;
}

// 移動状態を取得
EMoveState CSwitchMoveWall::GetMoveState() const
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