#include "CBuyMenu.h"
#include "CSceneManager.h"
#include "CJobUnlockMenu.h"
#include "CJobUpgradeMenu.h"
#include "CStageSelectMenu.h"
#include "CTextUI2D.h"

#define MENU_JOB_UNLOCK "UI/menu_job_unlock.png"
#define MENU_JOB_UPGRADE "UI/menu_job_upgrade.png"
#define MENU_JOB_SELECT "UI/menu_job_select.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// ���ʉ��̉���
#define SE_VOLUME 1.0f

// �����e�L�X�g�̍��W
#define UNLOCK_TEXT_POS		CVector(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.15f, 0.0f)
#define UPGRADE_TEXT_POS	CVector(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.35f, 0.0f)
#define SELECT_TEXT_POS		CVector(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.55f, 0.0f)
// �����e�L�X�g�̓��e
#define UNLOCK_TEXT		"�d�����������\n"
#define UPGRADE_TEXT	"�d���̓��e����������\n�i�������j\n"
#define SELECT_TEXT		"�s���d����I������\n"

// �R���X�g���N�^
CBuyMenu::CBuyMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_JOB_UNLOCK, MENU_JOB_UPGRADE, MENU_JOB_SELECT, MENU_CLOSE},
		MENU_SELECT)
{
	// �d���A�����b�N���j���[���쐬
	mpJobUnlockMenu = new CJobUnlockMenu(this);

	// �d���������j���[���쐬
	mpJobUpgradeMenu = new CJobUpgradeMenu(this);
	// ����
	SetMenuOnOff(1, false);
	
	// �d���I�����j���[���쐬
	mpStageSelectMenu = new CStageSelectMenu(this);


	// �����e�L�X�g��ݒ�
	mMenuTexts[0]->SetStr(UNLOCK_TEXT);
	mMenuTexts[1]->SetStr(UPGRADE_TEXT);
	mMenuTexts[2]->SetStr(SELECT_TEXT);
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
		// �L���Ȃ�J��
		if (mMenuOnOff[select])
		{
			Close();
			mpJobUnlockMenu->Open();
		}
		// �����Ȃ�u�U�[��
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;

	case 1:		// �����w�����j���[��\��
		// �L���Ȃ�J��
		if (mMenuOnOff[select])
		{
			Close();
			mpJobUpgradeMenu->Open();
		}
		// �����Ȃ�u�U�[��
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;

	case 2:		// �d���I�����j���[��\��
		// �L���Ȃ�J��
		if (mMenuOnOff[select])
		{
			Close();
			mpStageSelectMenu->Open();
		}
		// �����Ȃ�u�U�[��
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
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
