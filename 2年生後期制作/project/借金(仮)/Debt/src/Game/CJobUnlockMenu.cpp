#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// 効果音の音量
#define SE_VOLUME 1.0f

// コンストラクタ
CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;
	// 配達は無効
	SetMenuOnOff(1, false);
}

// デストラクタ
CJobUnlockMenu::~CJobUnlockMenu()
{
}

// 決定処理
void CJobUnlockMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// ゴミ拾いをアンロック
		// 有効ならアンロック
		if (mMenuOnOff[select])
		{
			CJobStatusManager::Instance()->SetUnlock(EJobType::eTrash, true);
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	case 1:		// 配達をアンロック
		// 有効ならアンロック
		if (mMenuOnOff[select])
		{
			CJobStatusManager::Instance()->SetUnlock(EJobType::eDelivery, true);
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	default:	// 一つ前のメニューへ戻る
		Close();
		mpPrevMenu->Open();
		break;
	}
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
	}
	// 配達がアンロック済みだったら
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
	{
		// 無効
		SetMenuOnOff(1, false);
	}
}
