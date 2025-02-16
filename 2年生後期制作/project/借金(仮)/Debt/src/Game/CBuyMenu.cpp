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

// ���ʉ��̉���
#define SE_VOLUME 0.5f

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
	: CGameMenuBase(std::vector<std::string> {MENU_JOB_UNLOCK, MENU_JOB_UPGRADE, MENU_JOB_SELECT, MENU_CLOSE})
{
	// �d���A�����b�N���j���[���쐬
	mpJobUnlockMenu = new CJobUnlockMenu(this);

	// �d���������j���[���쐬
	mpJobUpgradeMenu = new CJobUpgradeMenu(this);
	// ����
	SetMenuOnOff(1, false);
	
	// �d���I�����j���[���쐬
	mpStageSelectMenu = new CStageSelectMenu(this);


	// �Ō�̗v�f�ȊO�̃N���b�N���̃R�[���o�b�N�֐���ݒ�
	mButtons[0]->SetOnClickFunc(std::bind(&CBuyMenu::OnClickUnlock, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CBuyMenu::OnClickUpgrade, this));
	mButtons[2]->SetOnClickFunc(std::bind(&CBuyMenu::OnClickSelect, this));
	// �����e�L�X�g��ݒ�
	mMenuTexts[0]->SetStr(UNLOCK_TEXT);
	mMenuTexts[1]->SetStr(UPGRADE_TEXT);
	mMenuTexts[2]->SetStr(SELECT_TEXT);
}

// �f�X�g���N�^
CBuyMenu::~CBuyMenu()
{
}

// �X�V
void CBuyMenu::Update()
{
	CGameMenuBase::Update();
}

// [UNLOCK]�N���b�N���̃R�[���o�b�N�֐�
void CBuyMenu::OnClickUnlock()
{
	// �L���Ȃ�J��
	if (mMenuOnOff[0])
	{
		// �v�b�V����
		mpPushSE->Play(SE_VOLUME, true);
		Close();
		mpJobUnlockMenu->Open();
	}
	// �����Ȃ�u�U�[��
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// [UPGRADE]�N���b�N���̃R�[���o�b�N�֐�
void CBuyMenu::OnClickUpgrade()
{
	// �L���Ȃ�J��
	if (mMenuOnOff[1])
	{
		// �v�b�V����
		mpPushSE->Play(SE_VOLUME, true);
		Close();
		mpJobUpgradeMenu->Open();
	}
	// �����Ȃ�u�U�[��
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// [SELECT]�N���b�N���̃R�[���o�b�N�֐�
void CBuyMenu::OnClickSelect()
{
	// �L���Ȃ�J��
	if (mMenuOnOff[2])
	{
		// �v�b�V����
		mpPushSE->Play(SE_VOLUME, true);
		Close();
		mpStageSelectMenu->Open();
	}
	// �����Ȃ�u�U�[��
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}
