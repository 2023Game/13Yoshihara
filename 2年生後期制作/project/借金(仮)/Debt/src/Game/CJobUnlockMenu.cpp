#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"
#include "CTextUI2D.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// ���ʉ��̉���
#define SE_VOLUME 1.0f

// �����e�L�X�g�̓��e
#define TRASH_UNLOCK_TRUE_TEXT		"�S�~�E�����������\n�i����ρj"
#define TRASH_UNLOCK_FALSE_TEXT		"�S�~�E�����������\n�i������j"
#define DELIVERY_UNLOCK_TRUE_TEXT	"�z�B���������\n�i����ρj\n"
#define DELIVERY_UNLOCK_FALSE_TEXT	"�z�B���������\n�i������j\n"

// �R���X�g���N�^
CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;

	// �����e�L�X�g�̐ݒ�
	mMenuTexts[0]->SetStr(TRASH_UNLOCK_FALSE_TEXT);
	mMenuTexts[1]->SetStr(DELIVERY_UNLOCK_FALSE_TEXT);
}

// �f�X�g���N�^
CJobUnlockMenu::~CJobUnlockMenu()
{
}

// ���菈��
void CJobUnlockMenu::Decide(int select)
{
	switch (select)
	{
	case 0:		// �S�~�E�����A�����b�N
		// �L���Ȃ�A�����b�N
		if (mMenuOnOff[select])
		{
			CJobStatusManager::Instance()->SetUnlock(EJobType::eTrash, true);
		}
		// �����Ȃ�u�U�[��
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	case 1:		// �z�B���A�����b�N
		// �L���Ȃ�A�����b�N
		if (mMenuOnOff[select])
		{
			CJobStatusManager::Instance()->SetUnlock(EJobType::eDelivery, true);
		}
		// �����Ȃ�u�U�[��
		else
		{
			mpBuzzerSE->Play(SE_VOLUME, true);
		}
		break;
	default:	// ��O�̃��j���[�֖߂�
		Close();
		mpPrevMenu->Open();
		break;
	}
}

// �X�V
void CJobUnlockMenu::Update()
{
	CGameMenuBase::Update();
	// �S�~�E�����A�����b�N�ς݂�������
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eTrash))
	{
		// ����
		SetMenuOnOff(0, false);
		// �S�~�E��������ς݃e�L�X�g�ɕύX
		mMenuTexts[0]->SetStr(TRASH_UNLOCK_TRUE_TEXT);
	}
	// �z�B���A�����b�N�ς݂�������
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
	{
		// ����
		SetMenuOnOff(1, false);
		// �z�B������ς݃e�L�X�g�ɕύX
		mMenuTexts[1]->SetStr(DELIVERY_UNLOCK_TRUE_TEXT);
	}
}
