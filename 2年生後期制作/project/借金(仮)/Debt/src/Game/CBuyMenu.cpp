#include "CBuyMenu.h"
#include "CSceneManager.h"
#include "CJobUnlockMenu.h"
#include "CJobUpgradeMenu.h"

#define MENU_JOB_UNLOCK "UI/menu_job_unlock.png"
#define MENU_JOB_UPGRADE "UI/menu_job_upgrade.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

CBuyMenu::CBuyMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_JOB_UNLOCK, MENU_JOB_UPGRADE, MENU_CLOSE}, MENU_SELECT)
{
	// 仕事解放メニューを作成
	mpJobUnlockMenu = new CJobUnlockMenu(this);

	// 仕事強化メニューを作成
	mpJobUpgradeMenu = new CJobUpgradeMenu(this);
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
		mpJobUnlockMenu->Open();
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
