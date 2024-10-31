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
	case 0:		// �����
		break;
	case 1:		// �����
		break;
	default:	// ��O�̃��j���[�֖߂�
		Close();
		mPrevMenu->Open();
		break;
	}
}

void CJobLiberationMenu::Update()
{
	CGameMenuBase::Update();
}
