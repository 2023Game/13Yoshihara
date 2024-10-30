#include "CStageMenu.h"
#include "CSceneManager.h"

#define MENU_ITEM1 "UI/menu_item.png"
#define MENU_SELECT "UI/menu_item_select.png"

CStageMenu::CStageMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath)
	: CGameMenuBase(menuItemPathList,menuSelectPath)
{

}

CStageMenu::~CStageMenu()
{
}

void CStageMenu::Decide(int select)
{
	mMenuItemPng.size();
	switch (select)
	{
	case 0:		// ‚ÖˆÚ“®
		Close();
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
		break;
	case 1:
		break;
	default:	// ˆê”Ô‰º‚Íƒƒjƒ…[‚ğ•Â‚¶‚é
		Close();
		break;
	}
}

void CStageMenu::Update()
{
	CGameMenuBase::Update();
}
