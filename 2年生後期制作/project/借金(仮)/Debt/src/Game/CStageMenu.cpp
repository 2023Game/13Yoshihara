#include "CStageMenu.h"
#include "CSceneManager.h"

CStageMenu::CStageMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath)
	: CGameMenuBase(menuItemPathList,menuSelectPath)
{

}

CStageMenu::~CStageMenu()
{
}

void CStageMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// ‚ÖˆÚ“®
		Close();
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
		break;
	case 1:		// ‚ÖˆÚ“®
		Close();
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
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
