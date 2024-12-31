#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// コンストラクタ
CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;
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
		CJobStatusManager::Instance()->SetUnlock(EJobType::eTrash, true);
		break;
	case 1:		// 配達をアンロック
		CJobStatusManager::Instance()->SetUnlock(EJobType::eDelivery, true);
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
}
