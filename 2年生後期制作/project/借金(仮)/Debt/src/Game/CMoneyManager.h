#pragma once

// 自分の貧富の所持金の量
#define POOR_MONEY 5000		// 貧乏
#define RICH_MONEY 50000	// 裕福

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

	// 前の日の所持金を設定する
	void SetPreMoney(int money);

	// 所持金を取得する
	int GetMoney() const;
	// 所持金を設定する
	void SetMoney(int money);

	// 返済額を取得する
	int GetDebtMoney() const;
	// 指定した日数の返済額を設定する
	void SetDebtMoney(int day);

	// 次の返済額を取得する
	int GetNextDebtMoney() const;
	// 指定した日数の翌日の返済額を次の返済額に設定する
	void SetNextDebtMoney(int day);

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

	// 1日分ロールバックする
	void Rollback();

	// ゲームクリアかどうか
	bool IsClear();

private:
	// お金の管理クラスのインスタンス
	static CMoneyManager* spInstance;

	// 前の日の所持金
	int mPreMoney;

	// 所持金
	int mMoney;
	// 返済額
	int mDebtMoney;
	// 次の返済額
	int mNextDebtMoney;
	// 日数
	int mDay;
	// 返済をしたか
	bool mDid;
};