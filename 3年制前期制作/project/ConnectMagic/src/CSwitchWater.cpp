#include "CSwitchWater.h"
#include "CWater.h"
#include <typeinfo>

constexpr float MOVE_TIME = 5.0f;

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct SwitchWaterSaveData {
	CVector pos;		// 現在座標
	float elapsedTime;	// 経過時間
	CSwitchObject::EState state;	// 状態
};
#pragma pack(pop)	// 設定を元に戻す

std::vector<char> CSwitchWater::SaveState() const
{
	SwitchWaterSaveData data;
	data.pos = mpWater->Position();
	data.elapsedTime = GetElapsedTime();
	data.state = GetState();

	// データをバイト列に変換して返す
	const char* dataPtr = reinterpret_cast<const char*>(&data);
	return std::vector<char>(dataPtr, dataPtr + sizeof(data));
}

void CSwitchWater::LoadState(const std::vector<char>& data)
{
	if (data.size() != sizeof(SwitchWaterSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const SwitchWaterSaveData* saveData = reinterpret_cast<const SwitchWaterSaveData*>(data.data());
	mpWater->Position(saveData->pos);
	SetElapsedTime(saveData->elapsedTime);
	SetState(saveData->state);
}

size_t CSwitchWater::GetTypeID() const
{
	return typeid(CSwitchWater).hash_code();
}

unsigned int CSwitchWater::GetUniqueInstanceID() const
{
	return mUniqueID;
}

CSwitchWater::CSwitchWater(const CVector& pos, const CVector& downPos, const CVector& scale)
	: CSwitchObject(ETaskPriority::eBackground)
	, mDefaultPos(pos)
	, mDownPos(downPos)
	, mElapsedTime(MOVE_TIME)
	, mUniqueID(CUIDManager::GenerateNewID())
{
	mpWater = new CWater(pos, scale);
}

CSwitchWater::~CSwitchWater()
{
}

void CSwitchWater::SetElapsedTime(float time)
{
	mElapsedTime = time;
}

float CSwitchWater::GetElapsedTime() const
{
	return mElapsedTime;
}

// 作用していないときの更新
void CSwitchWater::UpdateOff()
{
	mElapsedTime += Times::DeltaTime();

	CVector newPos = CVector::zero;

	if (MOVE_TIME <= mElapsedTime)
	{
		newPos = mDefaultPos;
		mElapsedTime = MOVE_TIME;
	}
	else
	{
		// 進行度
		float per = mElapsedTime / MOVE_TIME;
		// 進行度で補間する
		newPos = CVector::Lerp(mDownPos, mDefaultPos, per);
	}
	mpWater->Position(newPos);
}

// 作用しているときの更新
void CSwitchWater::UpdateOn()
{
	mElapsedTime += Times::DeltaTime();

	CVector newPos = CVector::zero;

	if (MOVE_TIME <= mElapsedTime)
	{
		newPos = mDownPos;
		mElapsedTime = MOVE_TIME;
	}
	else
	{
		// 進行度
		float per = mElapsedTime / MOVE_TIME;
		// 進行度で補間する
		newPos = CVector::Lerp(mDefaultPos, mDownPos, per);
	}
	mpWater->Position(newPos);

	CDebugPrint::Print("経過時間:%f\n", mElapsedTime);
}

void CSwitchWater::SetEnable(bool enable)
{
	CObjectBase::SetEnable(enable);
	mpWater->SetEnable(enable);
}

void CSwitchWater::ChangeState(EState state)
{
	if (mState != state)
	{
		mState = state;
		mElapsedTime = 0.0f;
	}
}
