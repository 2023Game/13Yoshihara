#include "CBuyMenu.h"
#include "CSceneManager.h"
#include "CJobLiberationMenu.h"
#include "CJobUpgradeMenu.h"

#define MENU_JOB_LIBERATION1 "UI/menu_item.png"
#define MENU_JOB_UPGRADE1 "UI/menu_item.png"
#define MENU_SELECT "UI/menu_item_select.png"

CBuyMenu::CBuyMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath)
	: CGameMenuBase(menuItemPathList, menuSelectPath)
{
	std::string menuSelect = MENU_SELECT;

	// 仕事解放メニュー
	std::vector<std::string> menuJobLiberation;
	menuJobLiberation.push_back(MENU_JOB_LIBERATION1);
	menuJobLiberation.push_back(MENU_JOB_LIBERATION1);
	menuJobLiberation.push_back(MENU_JOB_LIBERATION1);

	mpJobLiberationMenu = new CJobLiberationMenu(menuJobLiberation, menuSelect, this);

	// 仕事強化メニュー
	std::vector<std::string> menuUpgrade;
	menuUpgrade.push_back(MENU_JOB_UPGRADE1);
	menuUpgrade.push_back(MENU_JOB_UPGRADE1);
	menuUpgrade.push_back(MENU_JOB_UPGRADE1);

	mpJobUpgradeMenu = new CJobUpgradeMenu(menuUpgrade, menuSelect, this);
}

CBuyMenu::~CBuyMenu()
{
}

void CBuyMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// 仕事解放メニューを表示
		Close();
		mpJobLiberationMenu->Open();
		break;
	case 1:		// 強化購入メニューを表示
		Close();
		mpJobUpgradeMenu->Open();
		break;
	default:	// 一番下はメニューを閉じる
		Close();
		break;
	}
}

void CBuyMenu::Update()
{
	CGameMenuBase::Update();
}
