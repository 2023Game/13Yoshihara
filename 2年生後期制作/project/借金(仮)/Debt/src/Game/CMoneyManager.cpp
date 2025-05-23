#include "CMoneyManager.h"

// お金の管理クラスのインスタンス
CMoneyManager* CMoneyManager::spInstance = nullptr;

// 初期の所持金
#define DEFAULT_MONEY 5000
// 初期の返済額
#define DEFAULT_DEBT_MONEY 3000

// ゲームクリアの日数
#define CLEAR_DAY 3

// インスタンスを取得
CMoneyManager* CMoneyManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CMoneyManager();
	}
	return spInstance;
}

// インスタンスを破棄
void CMoneyManager::ClearInstance()
{
	SAFE_DELETE(spInstance);
}

// コンストラクタ
CMoneyManager::CMoneyManager()
	: mPreMoney(DEFAULT_MONEY)
	, mMoney(DEFAULT_MONEY)
	, mDebtMoney(DEFAULT_DEBT_MONEY)
	, mNextDebtMoney(DEFAULT_DEBT_MONEY)
	, mDay(1)
	, mDid(false)
{
	// 次の返済額を1日目に設定
	SetNextDebtMoney(mDay + 1);
}

// デストラクタ
CMoneyManager::~CMoneyManager()
{
	// インスタンスを削除
	spInstance = nullptr;
}

// 前の日の所持金を設定する
void CMoneyManager::SetPreMoney(int money)
{
	mPreMoney = money;
}

// 所持金を取得する
int CMoneyManager::GetMoney() const
{
	return mMoney;
}

// 所持金を設定する
void CMoneyManager::SetMoney(int money)
{
	mMoney = money;
}

// 返済額を取得する
int CMoneyManager::GetDebtMoney() const
{
	return mDebtMoney;
}

// 指定した日数の返済額を設定する
void CMoneyManager::SetDebtMoney(int day)
{
	mDebtMoney = DEFAULT_DEBT_MONEY * (day * day + 1);
	mNextDebtMoney = mDebtMoney;
	SetNextDebtMoney(day + 1);
}

// 次の返済額を取得する
int CMoneyManager::GetNextDebtMoney() const
{
	return mNextDebtMoney;
}

// 指定した日数の翌日の返済額を次の返済額に設定する
void CMoneyManager::SetNextDebtMoney(int day)
{
	mNextDebtMoney = DEFAULT_DEBT_MONEY * (day * day + 1);
}

// 日数を取得する
int CMoneyManager::GetDay() const
{
	return mDay;
}

// 日数を設定する
void CMoneyManager::SetDay(int day)
{
	mDay = day;
}

// 日数を経過させる
void CMoneyManager::DayPass()
{
	mDay++;
}

// 返済したか
bool CMoneyManager::GetDid() const
{
	return mDid;
}

// 返済したかを設定する
void CMoneyManager::SetDid(bool isDid)
{
	mDid = isDid;
}

// 1日分ロールバックする
void CMoneyManager::Rollback()
{
	// 前の日の所持金に設定
	mMoney = mPreMoney;
	// 日数を1日戻す
	SetDay(GetDay() - 1);
	// 1日戻した日数の借金額を設定
	SetDebtMoney(GetDay());
	// 返済していない
	SetDid(false);
}

// ゲームクリアかどうか
bool CMoneyManager::IsClear()
{
	// クリアの日数以降なら
	if (mDay >= CLEAR_DAY)
	{
		// クリア
		return true;
	}
	// そうでない場合クリアしていない
	return false;
}
