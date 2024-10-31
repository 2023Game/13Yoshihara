#include "CJobLiberationMenu.h"
#include "CSceneManager.h"

CJobLiberationMenu::CJobLiberationMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath, CGameMenuBase* prevMenu)
	: CGameMenuBase(menuItemPathList, menuSelectPath)
{
	mPrevMenu = prevMenu;
}

CJobLiberationMenu::~CJobLiberationMenu()
{
}

void CJobLiberationMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// ‚ð‰ð•ú
		break;
	case 1:		// ‚ð‰ð•ú
		break;
	default:	// ˆê‚Â‘O‚Ìƒƒjƒ…[‚Ö–ß‚é
		Close();
		mPrevMenu->Open();
		break;
	}
}

void CJobLiberationMenu::Update()
{
	CGameMenuBase::Update();
}
