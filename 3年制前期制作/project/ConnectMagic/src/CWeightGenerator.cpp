#include "CWeightGenerator.h"
#include "CWeight.h"
#include <typeinfo>
#include "CSaveManager.h"

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct WeightGeneratorSaveData {
	bool isOn;				// 作動しているか
	bool isWeightEnable;	// おもりが有効か
};
#pragma pack(pop)	// 設定を元に戻す

std::vector<char> CWeightGenerator::SaveState() const
{
	WeightGeneratorSaveData data;
	data.isOn = GetOnOff();
	data.isWeightEnable = mpWeight->IsEnable();

	// データをバイト配列に変換して返す
	const char* dataPtr = reinterpret_cast<const char*>(&data);
	return std::vector<char>(dataPtr, dataPtr + sizeof(data));
}

void CWeightGenerator::LoadState(const std::vector<char>& data)
{
	if (data.size() != sizeof(WeightGeneratorSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const WeightGeneratorSaveData* saveData = reinterpret_cast<const WeightGeneratorSaveData*>(data.data());
	SetOnOff(saveData->isOn);
	mpWeight->SetEnable(saveData->isWeightEnable);
}

size_t CWeightGenerator::GetTypeID() const
{
	return typeid(CWeightGenerator).hash_code();
}

unsigned int CWeightGenerator::GetUniqueInstanceID() const
{
	return mUniqueID;
}

CWeightGenerator::CWeightGenerator(const CVector& pos)
	: CSwitchObject(ETaskPriority::eBackground)
	, mUniqueID(CUIDManager::GenerateNewID())
{
	mpModel = CResourceManager::Get<CModel>("Generator");
	Position(pos);

	// 最初重りはオフ
	mpWeight = new CWeight(pos);
	mpWeight->SetEnable(false);
}

CWeightGenerator::~CWeightGenerator()
{
}

void CWeightGenerator::SetEnable(bool enable)
{
	CSaveManager* saveMgr = CSaveManager::Instance();
	CTask::SetEnable(enable);
	if (enable)
	{
		saveMgr->AddSavableObj(mpWeight);
	}
	else
	{
		saveMgr->DeleteSavableObj(mpWeight);
	}
}

// 作用していないときの処理
void CWeightGenerator::UpdateOff()
{
}

// 作用しているときの処理
void CWeightGenerator::UpdateOn()
{
	mpWeight->SetEnable(true);
}
