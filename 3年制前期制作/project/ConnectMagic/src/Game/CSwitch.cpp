#include "CSwitch.h"
#include "CSwitchFrame.h"
#include "CSwitchButton.h"
#include "CSwitchObject.h"
#include "CCrystal.h"
#include <typeinfo>

#pragma pack(push,1)// パディング無効化
// 保存するデータ構造
struct SwitchSaveData {
	bool isOn;			// スイッチが作動しているか
};
#pragma pack(pop)	// 設定を元に戻す

std::vector<char> CSwitch::SaveState() const
{
	SwitchSaveData data;
	data.isOn = GetOnOff();
	const char* dataPtr = reinterpret_cast<const char*>(&data);
	return std::vector<char>(dataPtr, dataPtr + sizeof(data));
}

void CSwitch::LoadState(const std::vector<char>& data)
{
	// データサイズのチェック
	if (data.size() != sizeof(SwitchSaveData))
	{
		return;
	}

	// バイト列を構造体に戻し、データを復元
	const SwitchSaveData* saveData = reinterpret_cast<const SwitchSaveData*>(data.data());
	SetOnOff(saveData->isOn);
}

size_t CSwitch::GetTypeID() const
{
	return typeid(CSwitch).hash_code();
}

unsigned int CSwitch::GetUniqueInstanceID() const
{
	return mUniqueID;
}

// コンストラクタ
CSwitch::CSwitch(CVector pos, bool isAttach, ESwitchType type)
	: mIsOn(false)
	, mpFrame(nullptr)
	, mpButton(nullptr)
	, mpCrystal(nullptr)
	, mSwitchType(type)
	, mUniqueID(CUIDManager::GenerateNewID())
{
	switch (mSwitchType)
	{
	case ESwitchType::eButton:
		mpFrame = new CSwitchFrame(pos, this);
		mpButton = new CSwitchButton(pos, this, isAttach);
		break;
	case ESwitchType::eBatteries:
		mpCrystal = new CCrystal(pos, this);
		break;
	}
}

// コンストラクタ
CSwitch::~CSwitch()
{
}

// 作用するオブジェクトを設定する
void CSwitch::SetActionObj(CSwitchObject* obj)
{
	mActionObjs.push_back(obj);
}

// 作用するオブジェクトにオンオフを知らせる
void CSwitch::SetOnOff(bool isOnOff)
{
	// 同じなら処理しない
	if (mIsOn == isOnOff) return;
	mIsOn = isOnOff;

	for (int i = 0; i < mActionObjs.size(); i++)
	{
		// オンオフを切り替える
		mActionObjs[i]->SetOnOff(isOnOff);
	}
}

// スイッチが作動中かを取得
bool CSwitch::GetOnOff() const
{
	return mIsOn;
}

// スイッチのフレームを取得
CSwitchFrame* CSwitch::GetFrame()
{
	return mpFrame;
}

// スイッチのボタンを取得
CSwitchButton* CSwitch::GetButton()
{
	return mpButton;
}

// クリスタルを取得する
CCrystal* CSwitch::GetCrystal()
{
	return mpCrystal;
}

// スイッチの有効無効を切り替え
void CSwitch::SetEnableSwitch(bool enable)
{
	switch (mSwitchType)
	{
	case ESwitchType::eButton:
		mpFrame->SetEnable(enable);
		mpButton->SetEnable(enable);
		break;
	case ESwitchType::eBatteries:
		mpCrystal->SetEnable(enable);
		break;
	}
}