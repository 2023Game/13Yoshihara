#include "CBuyMenu.h"
#include "CSceneManager.h"
#include "CJobUnlockMenu.h"
#include "CJobUpgradeMenu.h"
#include "CStageSelectMenu.h"

#define MENU_JOB_UNLOCK "UI/menu_job_unlock.png"
#define MENU_JOB_UPGRADE "UI/menu_job_upgrade.png"
#define MENU_JOB_SELECT "UI/menu_job_select.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// �R���X�g���N�^
CBuyMenu::CBuyMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_JOB_UNLOCK, MENU_JOB_UPGRADE, MENU_JOB_SELECT, MENU_CLOSE},
		MENU_SELECT)
{
	// �d���A�����b�N���j���[���쐬
	mpJobUnlockMenu = new CJobUnlockMenu(this);

	// �d���������j���[���쐬
	mpJobUpgradeMenu = new CJobUpgradeMenu(this);
	
	// �d���I�����j���[���쐬
	mpStageSelectMenu = new CStageSelectMenu(this);
}

// �f�X�g���N�^
CBuyMenu::~CBuyMenu()
{
}

// ���菈��
void CBuyMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// �d���A�����b�N���j���[��\��
		Close();
		mpJobUnlockMenu->Open();
		break;
	case 1:		// �����w�����j���[��\��
		Close();
		mpJobUpgradeMenu->Open();
		break;
	case 2:		// �d���I�����j���[��\��
		Close();
		mpStageSelectMenu->Open();
		break;
	default:	// ��ԉ��̓��j���[�����
		Close();
		break;
	}
}

// �X�V
void CBuyMenu::Update()
{
	CGameMenuBase::Update();
}
