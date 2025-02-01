#include "CStageSelectMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_STAGE_TRASH "UI/menu_stage_trash.png"
#define MENU_STAGE_DELIVERY "UI/menu_stage_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// ���ʉ��̉���
#define SE_VOLUME 1.0f

// �R���X�g���N�^
CStageSelectMenu::CStageSelectMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_STAGE_TRASH, MENU_STAGE_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;
	// �Q�[���͂��ׂĖ���
	SetMenuOnOff(0, false);
	SetMenuOnOff(1, false);
}

// �f�X�g���N�^
CStageSelectMenu::~CStageSelectMenu()
{
}

// ���菈��
void CStageSelectMenu::Decide(int select)
{
	switch (select)
	{
	case 0:
		// �S�~�E�����L���Ȃ�
		if (mMenuOnOff[select])
		{
			// �I������Ă���d�����S�~�E���ɐݒ�
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eTrash);
		}
		// �����Ȃ�u�U�[��
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	case 1:
		// �z�B���L���Ȃ�
		if (mMenuOnOff[select])
		{
			// �I������Ă���d����z�B�ɐݒ�
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eDelivery);
		}
		// �����Ȃ�u�U�[��
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	default:	// ��O�̃��j���[�ɖ߂�
		Close();
		mpPrevMenu->Open();
		break;
	}
}

// �X�V
void CStageSelectMenu::Update()
{
	CGameMenuBase::Update();
	// �S�~�E�����A�����b�N�ς݂�������
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
	{
		// �L��
		SetMenuOnOff(0, true);
	}
	// �z�B���A�����b�N�ς݂�������
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
	{
		// �L��
		SetMenuOnOff(1, true);
	}
}
