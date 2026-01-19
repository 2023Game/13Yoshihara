#include "CSwitchMoveAirObj.h"
#include "Maths.h"
#include "CAirConnectObj.h"
#include <typeinfo>

// 移動速度
constexpr float MOVE_SPEED =	20.0f;
// 近い距離
constexpr float NEAR_DIST =		1.0f;
// 一時停止の時間
constexpr float STOP_TIME =		0.5f;

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct SwitchMoveAirObjSaveData {
	CVector pos;				// 現在座標
	float elapsedTime;			// 経過時間
	EMoveState moveState;		// 移動の状態
	CSwitchObject::EState state;// オンオフの状態
	int targetPosNum;			// 目標座標の番号
};
#pragma pack(pop)	// 設定を元に戻す


std::vector<char> CSwitchMoveAirObj::SaveState() const
{
	SwitchMoveAirObjSaveData data;
	data.pos = Position();
	data.elapsedTime = GetElapsedTime();
	data.moveState = GetMoveState();
	data.state = GetState();
	data.targetPosNum = GetTargetPosNum();

	// データをバイト列に変換して返す
	const char* dataPtr = reinterpret_cast<const char*>(&data);
	return std::vector<char>(dataPtr, dataPtr + sizeof(data));
}

void CSwitchMoveAirObj::LoadState(const std::vector<char>& data)
{
	// データのサイズチェック
	if (data.size() != sizeof(SwitchMoveAirObjSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const SwitchMoveAirObjSaveData* saveData = reinterpret_cast<const SwitchMoveAirObjSaveData*>(data.data());
	Position(saveData->pos);
	SetElapsedTime(saveData->elapsedTime);
	SetMoveState(saveData->moveState);
	SetState(saveData->state);
	SetTargetPosNum(saveData->targetPosNum);
}

size_t CSwitchMoveAirObj::GetTypeID() const
{
	return typeid(CSwitchMoveAirObj).hash_code();
}

unsigned int CSwitchMoveAirObj::GetUniqueInstanceID() const
{
	return mUniqueID;
}

// 移動状態を設定
void CSwitchMoveAirObj::SetMoveState(EMoveState state)
{
	mMoveState = state;
}

// 移動状態を取得
EMoveState CSwitchMoveAirObj::GetMoveState() const
{
	return mMoveState;
}

// 経過時間を設定
void CSwitchMoveAirObj::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

// 経過時間を取得
float CSwitchMoveAirObj::GetElapsedTime() const
{
	return mElapsedTime;
}

// コンストラクタ
CSwitchMoveAirObj::CSwitchMoveAirObj(const CVector& pos)
	: CSwitchObject()
	, mMoveState(EMoveState::eStop)
	, mElapsedTime(0.0f)
	, mpConnectObj(nullptr)
	, mTargetPosNum(0)
	, mUniqueID(CUIDManager::GenerateNewID())
{
	// 座標を設定
	Position(pos);
	// 空中接続オブジェクトを生成
	mpConnectObj = new CAirConnectObj(pos);
}

// デストラクタ
CSwitchMoveAirObj::~CSwitchMoveAirObj()
{
}

// 空中オブジェクトを取得
CAirConnectObj* CSwitchMoveAirObj::GetAirConnectObj()
{
	return mpConnectObj;
}

// 有効無効の切り替え
void CSwitchMoveAirObj::SetEnable(bool enable)
{
	CObjectBase::SetEnable(enable);
	mpConnectObj->SetEnable(enable);
}

void CSwitchMoveAirObj::AddTargetPos(const CVector& pos)
{
	mTargetPos.push_back(pos);
}

// 作用していないときの処理
void CSwitchMoveAirObj::UpdateOff()
{
}

// 作用しているときの処理
void CSwitchMoveAirObj::UpdateOn()
{
	Move();
}

// 移動の処理
void CSwitchMoveAirObj::Move()
{
	// 今の座標
	CVector pos = Position();
	// 目標座標
	CVector targetPos = mTargetPos[mTargetPosNum];

	// 移動方向
	CVector dir = targetPos - pos;
	// 距離
	float dist = dir.Length();

	// 到達した
	if (dist < NEAR_DIST)
	{
		mElapsedTime += Times::DeltaTime();
		// 一時停止の時間が経過したら
		if (mElapsedTime >= STOP_TIME)
		{
			mElapsedTime = 0.0f;
			// 座標を目標座標に設定
			Position(targetPos);
			// 要素番号を次へ
			mTargetPosNum++;
			// 要素数以上なら最初へ
			if (mTargetPosNum >= mTargetPos.size())
			{
				mTargetPosNum = 0;
			}
		}
	}
	// 到達していない
	else
	{
		dir.Normalize();
		// 新しい座標を計算
		CVector newPos = pos + dir * MOVE_SPEED * Times::DeltaTime();
		Position(newPos);
		mpConnectObj->Position(newPos);
	}
}

void CSwitchMoveAirObj::SetTargetPosNum(int num)
{
	mTargetPosNum = num;
}

int CSwitchMoveAirObj::GetTargetPosNum() const
{
	return mTargetPosNum;
}

// 状態を変更
void CSwitchMoveAirObj::ChangeMoveState(EMoveState state)
{
	if (mMoveState == state) return;

	// 状態を変更
	mMoveState = state;
	mElapsedTime = 0.0f;
}
