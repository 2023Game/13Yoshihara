#include "CStageSelectMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_STAGE_TRASH "UI/menu_stage_trash.png"
#define MENU_STAGE_DELIVERY "UI/menu_stage_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

CStageSelectMenu::CStageSelectMenu()
	: CGameMenuBase(std::vector<std::string> {MENU_STAGE_TRASH, MENU_STAGE_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{

}

CStageSelectMenu::~CStageSelectMenu()
{
}

void CStageSelectMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// �S�~�E���V�[���ֈړ�
		// �S�~�E�����A�����b�N�ς݂�������ړ�
		if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
		{
			Close();
			CSceneManager::Instance()->LoadScene(EScene::eTrashGame);
		}
		// �A�����b�N���Ă��Ȃ��Ȃ�u�U�[�����Đ�
		else
		{
			// TODO : �u�U�[�����Đ�
		}
		break;
	case 1:		// �z�B�V�[���ֈړ�
		// �z�B���A�����b�N�ς݂�������ړ�
		if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
		{
			Close();
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
		// �A�����b�N���Ă��Ȃ��Ȃ�u�U�[�����Đ�
		else
		{
			// TODO : �u�U�[�����Đ�
		}
		break;
	default:	// ��ԉ��̓��j���[�����
		Close();
		break;
	}
}

void CStageSelectMenu::Update()
{
	CGameMenuBase::Update();
}
