#include "CMoneyManager.h"

// お金の管理クラスのインスタンス
CMoneyManager* CMoneyManager::spInstance = nullptr;

// 初期の所持金
#define DEFAULT_MONEY 3000

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
	: mMoney(DEFAULT_MONEY)
{
}

// デストラクタ
CMoneyManager::~CMoneyManager()
{
	// インスタンスを削除
	spInstance = nullptr;
}

// 所持金を取得する
int CMoneyManager::GetMoney() const
{
	return mMoney;
}

// 所持金を加算する
void CMoneyManager::SetMoney(int money)
{
	mMoney += money;
	// 0より小さくなったら0にする
	if (mMoney < 0)
	{
		mMoney = 0;
	}
}
