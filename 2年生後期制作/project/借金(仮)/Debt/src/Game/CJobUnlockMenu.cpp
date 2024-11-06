#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mPrevMenu = prevMenu;
}

CJobUnlockMenu::~CJobUnlockMenu()
{
}

void CJobUnlockMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// �S�~�E�����A�����b�N
		CJobStatusManager::Instance()->SetUnlock(EJobType::eTrash, true);
		break;
	case 1:		// �z�B���A�����b�N
		CJobStatusManager::Instance()->SetUnlock(EJobType::eDelivery, true);
		break;
	default:	// ��O�̃��j���[�֖߂�
		Close();
		mPrevMenu->Open();
		break;
	}
}

void CJobUnlockMenu::Update()
{
	CGameMenuBase::Update();
}
