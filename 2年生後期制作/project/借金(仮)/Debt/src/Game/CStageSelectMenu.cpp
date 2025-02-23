#include "CStageSelectMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_STAGE_TRASH "UI/menu_stage_trash.png"
#define MENU_STAGE_DELIVERY "UI/menu_stage_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"

// ���ʉ��̉���
#define SE_VOLUME 0.5f

// �����e�L�X�g�̓��e
#define TRASH_TRUE_TEXT		"�S�~�E����I��\n�i����ρj\n"
#define TRASH_FALSE_TEXT	"�S�~�E����I��\n�i������j\n"
#define TRASH_SELECT_TEXT	"�S�~�E����I��\n�i�I���ρj\n"
#define DELIVERY_TRUE_TEXT		"�z�B��I��\n�i����ρj\n"
#define DELIVERY_FALSE_TEXT		"�z�B��I��\n�i������j\n"
#define DELIVERY_SELECT_TEXT	"�z�B��I��\n�i�I���ρj\n"

// �R���X�g���N�^
CStageSelectMenu::CStageSelectMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_STAGE_TRASH, MENU_STAGE_DELIVERY, MENU_CLOSE})
{
	mpPreMenu = prevMenu;
	// �Q�[���͂��ׂĖ���
	SetMenuOnOff(0, false);
	SetMenuOnOff(1, false);

	// �Ō�̗v�f�ȊO�̃N���b�N���̃R�[���o�b�N�֐���ݒ�
	mButtons[0]->SetOnClickFunc(std::bind(&CStageSelectMenu::OnClickTrash, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CStageSelectMenu::OnClickDelivery, this));
	// �����e�L�X�g�̐ݒ�
	mMenuTexts[0]->SetStr(TRASH_FALSE_TEXT);
	mMenuTexts[1]->SetStr(DELIVERY_FALSE_TEXT);
}

// �f�X�g���N�^
CStageSelectMenu::~CStageSelectMenu()
{
}

// �X�V
void CStageSelectMenu::Update()
{
	CGameMenuBase::Update();
	CJobStatusManager* jobStatusMgr = CJobStatusManager::Instance();

	// �S�~�E����I���ς݂Ȃ�
	if (jobStatusMgr->GetSelectJob() == EJobType::eTrash)
	{
		// ����
		SetMenuOnOff(0, false);
		// �S�~�E����I���ς݂̃e�L�X�g�ɕύX
		mMenuTexts[0]->SetStr(TRASH_SELECT_TEXT);
	}
	// �S�~�E�����A�����b�N�ς݂�������
	else if (jobStatusMgr->GetUnlock(EJobType::eTrash))
	{
		// �L��
		SetMenuOnOff(0, true);
		// �S�~�E��������ς݂̃e�L�X�g�ɕύX
		mMenuTexts[0]->SetStr(TRASH_TRUE_TEXT);
	}

	// �z�B��I���ς݂Ȃ�
	if (jobStatusMgr->GetSelectJob() == EJobType::eDelivery)
	{
		// ����
		SetMenuOnOff(1, false);
		// �z�B��I���ς݂̃e�L�X�g�ɕύX
		mMenuTexts[1]->SetStr(DELIVERY_SELECT_TEXT);
	}
	// �z�B���A�����b�N�ς݂�������
	else if (jobStatusMgr->GetUnlock(EJobType::eDelivery))
	{
		// �L��
		SetMenuOnOff(1, true);
		// �z�B������ς݂̃e�L�X�g�ɕύX
		mMenuTexts[1]->SetStr(DELIVERY_TRUE_TEXT);
	}
}

// [TRASH]�N���b�N���̃R�[���o�b�N�֐�
void CStageSelectMenu::OnClickTrash()
{
	// �S�~�E�����L���Ȃ�
	if (mMenuOnOff[0])
	{
		// �v�b�V����
		mpPushSE->Play(SE_VOLUME, true);
		// �I������Ă���d�����S�~�E���ɐݒ�
		CJobStatusManager::Instance()->SetSelectJob(EJobType::eTrash);
	}
	// �����Ȃ�u�U�[��
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// [DELIVERY]�N���b�N���̃R�[���o�b�N�֐�
void CStageSelectMenu::OnClickDelivery()
{
	// �z�B���L���Ȃ�
	if (mMenuOnOff[1])
	{
		// �v�b�V����
		mpPushSE->Play(SE_VOLUME, true);
		// �I������Ă���d����z�B�ɐݒ�
		CJobStatusManager::Instance()->SetSelectJob(EJobType::eDelivery);
	}
	// �����Ȃ�u�U�[��
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}
