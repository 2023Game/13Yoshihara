#include "CGameMenu.h"
#include "CSceneManager.h"

CGameMenu::CGameMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath)
	: CGameMenuBase(menuItemPathList, menuSelectPath)
{

}

CGameMenu::~CGameMenu()
{
}

void CGameMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// タイトルへ移動
		Close();
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
		break;
	case 1:		// 
		Close();
		break;
	default:	// 一番下はメニューを閉じる
		Close();
		break;
	}
}

void CGameMenu::Update()
{
	CGameMenuBase::Update();
}
