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

	// �d��������j���[
	std::vector<std::string> menuJobLiberation;
	menuJobLiberation.push_back(MENU_JOB_LIBERATION1);
	menuJobLiberation.push_back(MENU_JOB_LIBERATION1);
	menuJobLiberation.push_back(MENU_JOB_LIBERATION1);

	mpJobLiberationMenu = new CJobLiberationMenu(menuJobLiberation, menuSelect, this);

	// �d���������j���[
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
	case 0:		// �d��������j���[��\��
		Close();
		mpJobLiberationMenu->Open();
		break;
	case 1:		// �����w�����j���[��\��
		Close();
		mpJobUpgradeMenu->Open();
		break;
	default:	// ��ԉ��̓��j���[�����
		Close();
		break;
	}
}

void CBuyMenu::Update()
{
	CGameMenuBase::Update();
}
