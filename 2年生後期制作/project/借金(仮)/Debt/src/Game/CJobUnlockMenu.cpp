#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"
#include "CTextUI2D.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"

// 効果音の音量
#define SE_VOLUME 0.5f

// 説明テキストの内容
#define UNLOCK_TRUE_TEXT		"（解放済）\n"
#define TRASH_UNLOCK_FALSE_TEXT		"ゴミ拾いを解放する\n2000円\n"
#define DELIVERY_UNLOCK_FALSE_TEXT	"配達を解放する\n2000円\n"

// コンストラクタ
CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE})
{
	mpPrevMenu = prevMenu;

	// 最後の要素以外のクリック時のコールバック関数を設定
	mButtons[0]->SetOnClickFunc(std::bind(&CJobUnlockMenu::OnClickTrash, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CJobUnlockMenu::OnClickDelivery, this));
	// 説明テキストの設定
	mMenuTexts[0]->SetStr(TRASH_UNLOCK_FALSE_TEXT);
	mMenuTexts[1]->SetStr(DELIVERY_UNLOCK_FALSE_TEXT);
}

// デストラクタ
CJobUnlockMenu::~CJobUnlockMenu()
{
}

// 更新
void CJobUnlockMenu::Update()
{
	CGameMenuBase::Update();
	// ゴミ拾いがアンロック済みだったら
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
	{
		// 無効
		SetMenuOnOff(0, false);
		// ゴミ拾いを解放済みテキストに変更
		mMenuTexts[0]->SetStr(UNLOCK_TRUE_TEXT);
	}
	// 配達がアンロック済みだったら
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
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
	// 有効ならアンロック
	if (mMenuOnOff[0])
	{
		// プッシュ音
		mpPushSE->Play(SE_VOLUME, true);
		CJobStatusManager::Instance()->SetUnlock(EJobType::eTrash, true);
	}
	// 無効ならブザー音
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// [DELIVERY]クリック時のコールバック関数
void CJobUnlockMenu::OnClickDelivery()
{
	// 配達
	// 有効ならアンロック
	if (mMenuOnOff[1])
	{
		// プッシュ音
		mpPushSE->Play(SE_VOLUME, true);
		CJobStatusManager::Instance()->SetUnlock(EJobType::eDelivery, true);
	}
	// 無効ならブザー音
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}
