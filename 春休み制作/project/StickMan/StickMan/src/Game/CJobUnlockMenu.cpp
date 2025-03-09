#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"
#include "CTextUI2D.h"
#include "CMoneyManager.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"

// 効果音の音量
#define SE_VOLUME 0.5f

// 説明テキストの内容
#define UNLOCK_TRUE_TEXT			"（解放済）\n"
#define TRASH_UNLOCK_FALSE_TEXT		"ゴミ拾いを解放する\n%d円\n"
#define DELIVERY_UNLOCK_FALSE_TEXT	"配達を解放する\n%d円\n"

// コンストラクタ
CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE})
{
	mpPreMenu = prevMenu;

	// 最後の要素以外のクリック時のコールバック関数を設定
	mButtons[0]->SetOnClickFunc(std::bind(&CJobUnlockMenu::OnClickTrash, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CJobUnlockMenu::OnClickDelivery, this));
	auto* jobStatusMgr = CJobStatusManager::Instance();
	int trashUnlockMoney = jobStatusMgr->GetUnlockMoney(EJobType::eTrash);
	int deliveryUnlockMoney = jobStatusMgr->GetUnlockMoney(EJobType::eDelivery);
	// 説明テキストの設定
	mMenuTexts[0]->ChangeToStr(TRASH_UNLOCK_FALSE_TEXT, trashUnlockMoney);
	mMenuTexts[1]->ChangeToStr(DELIVERY_UNLOCK_FALSE_TEXT, deliveryUnlockMoney);
}

// デストラクタ
CJobUnlockMenu::~CJobUnlockMenu()
{
}

// 更新
void CJobUnlockMenu::Update()
{
	CGameMenuBase::Update();
	// ジョブステータス管理クラス
	auto* jobMgr = CJobStatusManager::Instance();
	// ゴミ拾いがアンロック済みだったら
	if (jobMgr->GetUnlock(EJobType::eTrash))
	{
		// 無効
		SetMenuOnOff(0, false);
		// ゴミ拾いを解放済みテキストに変更
		mMenuTexts[0]->SetStr(UNLOCK_TRUE_TEXT);
	}
	// 配達がアンロック済みだったら
	if (jobMgr->GetUnlock(EJobType::eDelivery))
	{
		// 無効
		SetMenuOnOff(1, false);
		// 配達を解放済みテキストに変更
		mMenuTexts[1]->SetStr(UNLOCK_TRUE_TEXT);
	}
}

// [TRASH]クリック時のコールバック関数
void CJobUnlockMenu::OnClickTrash()
{
	// ゴミ拾い
	// 有効かつ所持金を減らせたらアンロック
	if (mMenuOnOff[0] &&
		IncreaseMoney(EJobType::eTrash))
	{
		// プッシュ音
		mpPushSE->Play(SE_VOLUME, true);
		// 仕事をアンロック
		CJobStatusManager::Instance()->SetUnlock(EJobType::eTrash, true);
	}
	// 無効か所持金が足りないならブザー音
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// [DELIVERY]クリック時のコールバック関数
void CJobUnlockMenu::OnClickDelivery()
{
	// 配達
	// 有効かつ所持金を減らせたらアンロック
	if (mMenuOnOff[1] &&
		IncreaseMoney(EJobType::eDelivery))
	{
		// プッシュ音
		mpPushSE->Play(SE_VOLUME, true);
		// 仕事をアンロック
		CJobStatusManager::Instance()->SetUnlock(EJobType::eDelivery, true);
	}
	// 無効か所持金が足りないならブザー音
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// アンロック金額を減らせたか
bool CJobUnlockMenu::IncreaseMoney(EJobType jobType)
{
	// 仕事があるか
	bool isJob = false;
	// お金の管理クラスを取得
	auto* moneyMgr = CMoneyManager::Instance();
	// 仕事管理クラスを取得
	auto* jobStatusMgr = CJobStatusManager::Instance();
	// 所持金
	int money = moneyMgr->GetMoney();
	// アンロック金額
	int unlockMoney = 0;

	// ゴミ拾いの場合
	if (jobType == EJobType::eTrash)
	{
		// アンロック金額を設定
		unlockMoney = jobStatusMgr->GetUnlockMoney(EJobType::eTrash);
		// 仕事がある
		isJob = true;
	}
	// 配達の場合
	else if (jobType == EJobType::eDelivery)
	{
		// アンロック金額を取得
		unlockMoney = jobStatusMgr->GetUnlockMoney(EJobType::eDelivery);
		// 仕事がある
		isJob = true;
	}
	// 仕事がないなら失敗
	if (!isJob) return false;

	// アンロック金額を減算
	money -= unlockMoney;
	// 所持金が0より小さいならアンロック失敗
	if (money < 0) return false;

	// 所持金を設定
	moneyMgr->SetMoney(money);
	// アンロック成功
	return true;
}
