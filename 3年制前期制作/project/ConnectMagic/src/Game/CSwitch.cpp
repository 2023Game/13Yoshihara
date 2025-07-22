#include "CSwitch.h"
#include "CSwitchFrame.h"
#include "CSwitchButton.h"
#include "CSwitchObject.h"

// コンストラクタ
CSwitch::CSwitch(CVector pos)
	: mpActionObject(nullptr)
	, mIsOn(false)
{
	mpFrame = new CSwitchFrame(pos, this);
	mpButton = new CSwitchButton(pos, this);
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
	if (mpActionObject == nullptr) return;
	// 同じなら処理しない
	if (mIsOn == isOnOff) return;
	mIsOn = isOnOff;

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
