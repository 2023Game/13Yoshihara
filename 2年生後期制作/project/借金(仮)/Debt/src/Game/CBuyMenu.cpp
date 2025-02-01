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

// 効果音の音量
#define SE_VOLUME 1.0f

// コンストラクタ
CBuyMenu::CBuyMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_JOB_UNLOCK, MENU_JOB_UPGRADE, MENU_JOB_SELECT, MENU_CLOSE},
		MENU_SELECT)
{
	// 仕事アンロックメニューを作成
	mpJobUnlockMenu = new CJobUnlockMenu(this);

	// 仕事強化メニューを作成
	mpJobUpgradeMenu = new CJobUpgradeMenu(this);
	// 無効
	SetMenuOnOff(1, false);
	
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
		// 有効なら開く
		if (mMenuOnOff[select])
		{
			Close();
			mpJobUnlockMenu->Open();
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;

	case 1:		// 強化購入メニューを表示
		// 有効なら開く
		if (mMenuOnOff[select])
		{
			Close();
			mpJobUpgradeMenu->Open();
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;

	case 2:		// 仕事選択メニューを表示
		// 有効なら開く
		if (mMenuOnOff[select])
		{
			Close();
			mpStageSelectMenu->Open();
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
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
