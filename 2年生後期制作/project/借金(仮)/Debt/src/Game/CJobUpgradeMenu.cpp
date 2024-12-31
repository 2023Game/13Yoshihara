#include "CJobUpgradeMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_UPGRADE_TRASH "UI/menu_upgrade_trash.png"
#define MENU_UPGRADE_DELIVERY "UI/menu_upgrade_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"
#define UPGRADE_FLOAT 0.1f

// �R���X�g���N�^
CJobUpgradeMenu::CJobUpgradeMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UPGRADE_TRASH, MENU_UPGRADE_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;
}

// �f�X�g���N�^
CJobUpgradeMenu::~CJobUpgradeMenu()
{
}

// ���菈��
void CJobUpgradeMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// �S�~�E��������
		CJobStatusManager::Instance()->SetUpgrade(EJobType::eTrash, UPGRADE_FLOAT);
		// TODO : �����������Đ�
		break;
	case 1:		// �z�B������
		CJobStatusManager::Instance()->SetUpgrade(EJobType::eDelivery, UPGRADE_FLOAT);
		// TODO : �����������Đ�
		break;
	default:	// ��O�̃��j���[�ɖ߂�
		Close();
		mpPrevMenu->Open();
		break;
	}
}

// �X�V
void CJobUpgradeMenu::Update()
{
	CGameMenuBase::Update();
}
