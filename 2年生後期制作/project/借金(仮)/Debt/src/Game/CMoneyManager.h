#pragma once

/*
お金の管理クラス
*/
class CMoneyManager
{
public:
	// インスタンスを取得
	static CMoneyManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	// コンストラクタ
	CMoneyManager();
	// デストラクタ
	~CMoneyManager();

	// 所持金を取得する
	int GetMoney() const;
	// 所持金を設定する
	void SetMoney(int money);

	// 返済額を取得する
	int GetDebtMoney() const;
	// 指定した日数の返済額を設定する
	void SetDebtMoney(int day);

	// 日数を取得する
	int GetDay() const;
	// 日数を設定する
	void SetDay(int day);
	// 日数を経過させる
	void DayPass();

	// 返済したか
	bool GetDid() const;
	// 返済したかを設定する
	void SetDid(bool isDid);

private:
	// お金の管理クラスのインスタンス
	static CMoneyManager* spInstance;

	// 所持金
	int mMoney;
	// 返済額
	int mDebtMoney;
	// 日数
	int mDay;
	// 返済をしたか
	bool mDid;
};