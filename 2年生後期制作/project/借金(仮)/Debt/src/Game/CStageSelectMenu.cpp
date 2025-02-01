#include "CStageSelectMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_STAGE_TRASH "UI/menu_stage_trash.png"
#define MENU_STAGE_DELIVERY "UI/menu_stage_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// 効果音の音量
#define SE_VOLUME 1.0f

// コンストラクタ
CStageSelectMenu::CStageSelectMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_STAGE_TRASH, MENU_STAGE_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;
	// ゲームはすべて無効
	SetMenuOnOff(0, false);
	SetMenuOnOff(1, false);
}

// デストラクタ
CStageSelectMenu::~CStageSelectMenu()
{
}

// 決定処理
void CStageSelectMenu::Decide(int select)
{
	switch (select)
	{
	case 0:
		// ゴミ拾いが有効なら
		if (mMenuOnOff[select])
		{
			// 選択されている仕事をゴミ拾いに設定
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eTrash);
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	case 1:
		// 配達が有効なら
		if (mMenuOnOff[select])
		{
			// 選択されている仕事を配達に設定
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eDelivery);
		}
		// 無効ならブザー音
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	default:	// 一つ前のメニューに戻る
		Close();
		mpPrevMenu->Open();
		break;
	}
}

// 更新
void CStageSelectMenu::Update()
{
	CGameMenuBase::Update();
	// ゴミ拾いがアンロック済みだったら
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
	{
		// 有効
		SetMenuOnOff(0, true);
	}
	// 配達がアンロック済みだったら
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
	{
		// 有効
		SetMenuOnOff(1, true);
	}
}
