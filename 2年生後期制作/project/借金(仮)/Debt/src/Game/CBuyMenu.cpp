#include "CBuyMenu.h"
#include "CSceneManager.h"
#include "CJobUnlockMenu.h"
#include "CJobUpgradeMenu.h"
#include "CStageSelectMenu.h"

#define MENU_JOB_UNLOCK "UI/menu_job_unlock.png"
#define MENU_JOB_UPGRADE "UI/menu_job_upgrade.png"
#define MENU_JOB_SELECT "UI/menu_job_select.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// コンストラクタ
CBuyMenu::CBuyMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_JOB_UNLOCK, MENU_JOB_UPGRADE, MENU_JOB_SELECT, MENU_CLOSE},
		MENU_SELECT)
{
	// 仕事アンロックメニューを作成
	mpJobUnlockMenu = new CJobUnlockMenu(this);

	// 仕事強化メニューを作成
	mpJobUpgradeMenu = new CJobUpgradeMenu(this);
	
	// 仕事選択メニューを作成
	mpStageSelectMenu = new CStageSelectMenu(this);
}

// デストラクタ
CBuyMenu::~CBuyMenu()
{
}

// 決定処理
void CBuyMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// 仕事アンロックメニューを表示
		Close();
		mpJobUnlockMenu->Open();
		break;
	case 1:		// 強化購入メニューを表示
		Close();
		mpJobUpgradeMenu->Open();
		break;
	case 2:		// 仕事選択メニューを表示
		Close();
		mpStageSelectMenu->Open();
		break;
	default:	// 一番下はメニューを閉じる
		Close();
		break;
	}
}

// 更新
void CBuyMenu::Update()
{
	CGameMenuBase::Update();
}
