#include "CJobUpgradeMenu.h"
#include "CSceneManager.h"

CJobUpgradeMenu::CJobUpgradeMenu(std::vector<std::string> menuItemPathList, std::string menuSelectPath, CGameMenuBase* prevMenu)
	: CGameMenuBase(menuItemPathList, menuSelectPath)
{
	mPrevMenu = prevMenu;
}

CJobUpgradeMenu::~CJobUpgradeMenu()
{
}

void CJobUpgradeMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// ������
		break;
	case 1:		// ������
		break;
	default:	// ��O�̃��j���[�ɖ߂�
		Close();
		mPrevMenu->Open();
		break;
	}
}

void CJobUpgradeMenu::Update()
{
	CGameMenuBase::Update();
}
