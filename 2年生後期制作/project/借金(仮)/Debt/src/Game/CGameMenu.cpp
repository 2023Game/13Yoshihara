#include "CGameMenu.h"
#include "CSceneManager.h"

#define MENU_ITEM1 "UI/menu_item.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// コンストラクタ
CGameMenu::CGameMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_ITEM1, MENU_ITEM1, MENU_CLOSE}, MENU_SELECT)
{
	
}

// デストラクタ
CGameMenu::~CGameMenu()
{
}

// 決定処理
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

// 更新
void CGameMenu::Update()
{
	CGameMenuBase::Update();
}
