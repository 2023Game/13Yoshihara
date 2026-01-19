#pragma once
#include "CSavable.h"

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
class CSwitch : public CSavable
{
public:
	// CSavableの純粋仮想関数のオーバーライド
	std::vector<char> SaveState() const override;
	void LoadState(const std::vector<char>& data) override;
	size_t GetTypeID() const override;
	unsigned int GetUniqueInstanceID() const override;

	// コンストラクタ
	CSwitch(const CVector& pos,
		const CVector& scale = CVector::one,
		bool isAttach = false,
		ESwitchType type = ESwitchType::eButton);
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

#if _DEBUG
	// スイッチと作用するオブジェクトの間に線を描画
	void Render();
#endif

private:
	// データ保存に使用
	unsigned int mUniqueID;

	CSwitchFrame* mpFrame;	// フレーム
	CSwitchButton* mpButton;// ボタン
	CCrystal* mpCrystal;	// クリスタル

	std::vector<CSwitchObject*> mActionObjs;

	ESwitchType mSwitchType;	// スイッチの種類

	// スイッチが作動しているか
	bool mIsOn;
};