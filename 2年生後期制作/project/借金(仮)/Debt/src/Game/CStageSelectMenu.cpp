#include "CStageSelectMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_STAGE_TRASH "UI/menu_stage_trash.png"
#define MENU_STAGE_DELIVERY "UI/menu_stage_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

CStageSelectMenu::CStageSelectMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_STAGE_TRASH, MENU_STAGE_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{

}

CStageSelectMenu::~CStageSelectMenu()
{
}

void CStageSelectMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// ゴミ拾いシーンへ移動
		// ゴミ拾いがアンロック済みだったら移動
		if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
		{
			Close();
			CSceneManager::Instance()->LoadScene(EScene::eTrashGame);
		}
		// アンロックしていないならブザー音を再生
		else
		{
			// TODO : ブザー音を再生
		}
		break;
	case 1:		// 配達シーンへ移動
		// 配達がアンロック済みだったら移動
		if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
		{
			Close();
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
		// アンロックしていないならブザー音を再生
		else
		{
			// TODO : ブザー音を再生
		}
		break;
	default:	// 一番下はメニューを閉じる
		Close();
		break;
	}
}

void CStageSelectMenu::Update()
{
	CGameMenuBase::Update();
}
