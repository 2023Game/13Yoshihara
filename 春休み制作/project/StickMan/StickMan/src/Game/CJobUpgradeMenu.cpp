#include "CJobUpgradeMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_UPGRADE_TRASH "UI/menu_upgrade_trash.png"
#define MENU_UPGRADE_DELIVERY "UI/menu_upgrade_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"
#define UPGRADE_FLOAT 0.1f

// �R���X�g���N�^
CJobUpgradeMenu::CJobUpgradeMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UPGRADE_TRASH, MENU_UPGRADE_DELIVERY, MENU_CLOSE})
{
	mpPreMenu = prevMenu;

	// �Ō�̗v�f�ȊO�̃N���b�N���̃R�[���o�b�N�֐���ݒ�
	mButtons[0]->SetOnClickFunc(std::bind(&CJobUpgradeMenu::OnClickTrash, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CJobUpgradeMenu::OnClickDelivery, this));
}

// �f�X�g���N�^
CJobUpgradeMenu::~CJobUpgradeMenu()
{
}

// �X�V
void CJobUpgradeMenu::Update()
{
	CGameMenuBase::Update();
}

// [TRASH]�N���b�N���̃R�[���o�b�N�֐�
void CJobUpgradeMenu::OnClickTrash()
{
	// �S�~�E��������
	CJobStatusManager::Instance()->SetBonus(EJobType::eTrash, UPGRADE_FLOAT);
	// TODO : �����������Đ�
}

// [DELIVERY]�N���b�N���̃R�[���o�b�N�֐�
void CJobUpgradeMenu::OnClickDelivery()
{
	// �z�B������
	CJobStatusManager::Instance()->SetBonus(EJobType::eDelivery, UPGRADE_FLOAT);
	// TODO : �����������Đ�
}
