#include "CStageSelectMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_STAGE_TRASH "UI/menu_stage_trash.png"
#define MENU_STAGE_DELIVERY "UI/menu_stage_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// コンストラクタ
CStageSelectMenu::CStageSelectMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_STAGE_TRASH, MENU_STAGE_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;
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
		// ゴミ拾いがアンロック済みだったら
		if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
		{
			// 選択されている仕事をゴミ拾いに設定
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eTrash);
		}
		// アンロックしていないならブザー音を再生
		else
		{
			// TODO : ブザー音を再生
		}
		break;
	case 1:
		// 配達がアンロック済みだったら
		if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
		{
			// 選択されている仕事を配達に設定
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eDelivery);
		}
		// アンロックしていないならブザー音を再生
		else
		{
			// TODO : ブザー音を再生
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
}
