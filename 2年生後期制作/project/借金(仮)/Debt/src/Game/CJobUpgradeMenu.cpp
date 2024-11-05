#include "CJobUpgradeMenu.h"
#include "CSceneManager.h"

#define MENU_UPGRADE_TRASH "UI/menu_upgrade_trash.png"
#define MENU_UPGRADE_DELIVERY "UI/menu_upgrade_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

CJobUpgradeMenu::CJobUpgradeMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UPGRADE_TRASH, MENU_UPGRADE_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mPrevMenu = prevMenu;
}

CJobUpgradeMenu::~CJobUpgradeMenu()
{
}

void CJobUpgradeMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// ゴミ拾いを強化
		break;
	case 1:		// 配達を強化
		break;
	default:	// 一つ前のメニューに戻る
		Close();
		mPrevMenu->Open();
		break;
	}
}

void CJobUpgradeMenu::Update()
{
	CGameMenuBase::Update();
}
