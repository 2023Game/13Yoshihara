#include "CSwitch.h"
#include "CSwitchFrame.h"
#include "CSwitchButton.h"
#include "CSwitchObject.h"
#include "CCrystal.h"

// コンストラクタ
CSwitch::CSwitch(CVector pos, bool isAttach, ESwitchType type)
	: mpActionObject(nullptr)
	, mIsOn(false)
	, mpFrame(nullptr)
	, mpButton(nullptr)
	, mpCrystal(nullptr)
	, mSwitchType(type)
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
	mpActionObject = obj;
}

// 作用するオブジェクトにオンオフを知らせる
void CSwitch::SetOnOff(bool isOnOff)
{
	// 同じなら処理しない
	if (mIsOn == isOnOff) return;
	mIsOn = isOnOff;

	if (mpActionObject == nullptr) return;
	// オンオフを切り替える
	mpActionObject->SetOnOff(isOnOff);
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