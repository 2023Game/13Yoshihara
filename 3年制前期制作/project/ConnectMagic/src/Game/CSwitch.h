#pragma once

class CSwitchFrame;
class CSwitchButton;
class CSwitchObject;

// スイッチのクラス
class CSwitch
{
public:
	// コンストラクタ
	CSwitch(CVector pos);
	// デストラクタ
	~CSwitch();

	// 作用するオブジェクトを設定する
	void SetActionObj(CSwitchObject* obj);

	// 作用するオブジェクトにオンオフを知らせる
	void SetOnOff(bool isOnOff);
	// スイッチが作動中かを取得
	bool GetOnOff() const;

	// スイッチのフレームを取得
	CSwitchFrame* GetFrame();
	// スイッチのボタンを取得
	CSwitchButton* GetButton();

private:
	CSwitchFrame* mpFrame;	// フレーム
	CSwitchButton* mpButton;// ボタン

	CSwitchObject* mpActionObject;	// 作用するオブジェクト

	// スイッチが作動しているか
	bool mIsOn;
};