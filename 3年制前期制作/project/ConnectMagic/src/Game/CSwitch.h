#pragma once

class CSwitchFrame;
class CSwitchButton;
class CSwitchObject;
class CCrystal;

// スイッチのタイプ
enum class ESwitchType
{
	eButton,	// ボタン式
	eBatteries,	// 電源式
};

// スイッチのクラス
class CSwitch
{
public:

	// コンストラクタ
	CSwitch(CVector pos, bool isAttach = false, ESwitchType type = ESwitchType::eButton);
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
	// クリスタルを取得する
	CCrystal* GetCrystal();

	// スイッチの有効無効を切り替え
	void SetEnableSwitch(bool enable);

private:
	CSwitchFrame* mpFrame;	// フレーム
	CSwitchButton* mpButton;// ボタン
	CCrystal* mpCrystal;	// クリスタル


	CSwitchObject* mpActionObject;	// 作用するオブジェクト

	ESwitchType mSwitchType;	// スイッチの種類

	// スイッチが作動しているか
	bool mIsOn;
};