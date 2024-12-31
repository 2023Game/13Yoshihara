#include "CStageSelectMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_STAGE_TRASH "UI/menu_stage_trash.png"
#define MENU_STAGE_DELIVERY "UI/menu_stage_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// �R���X�g���N�^
CStageSelectMenu::CStageSelectMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_STAGE_TRASH, MENU_STAGE_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;
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
		// �S�~�E�����A�����b�N�ς݂�������
		if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
		{
			// �I������Ă���d�����S�~�E���ɐݒ�
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eTrash);
		}
		// �A�����b�N���Ă��Ȃ��Ȃ�u�U�[�����Đ�
		else
		{
			// TODO : �u�U�[�����Đ�
		}
		break;
	case 1:
		// �z�B���A�����b�N�ς݂�������
		if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
		{
			// �I������Ă���d����z�B�ɐݒ�
			CJobStatusManager::Instance()->SetSelectJob(EJobType::eDelivery);
		}
		// �A�����b�N���Ă��Ȃ��Ȃ�u�U�[�����Đ�
		else
		{
			// TODO : �u�U�[�����Đ�
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
}
