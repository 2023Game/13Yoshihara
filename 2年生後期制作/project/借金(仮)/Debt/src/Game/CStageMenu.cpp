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
	case 0:		// へ移動
		Close();
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
		break;
	case 1:		// へ移動
		Close();
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
		break;
	default:	// 一番下はメニューを閉じる
		Close();
		break;
	}
}

void CStageMenu::Update()
{
	CGameMenuBase::Update();
}
