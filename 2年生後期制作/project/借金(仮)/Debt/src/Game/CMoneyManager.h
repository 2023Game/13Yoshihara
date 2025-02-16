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
	// 所持金を加算する
	void SetMoney(int money);

private:
	// お金の管理クラスのインスタンス
	static CMoneyManager* spInstance;

	// 所持金
	int mMoney;
};