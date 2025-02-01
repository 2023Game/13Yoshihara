#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"
#define MENU_CLOSE "UI/menu_close.png"
#define MENU_SELECT "UI/menu_select.png"

// ���ʉ��̉���
#define SE_VOLUME 1.0f

// �R���X�g���N�^
CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE}, MENU_SELECT)
{
	mpPrevMenu = prevMenu;
	// �z�B�͖���
	SetMenuOnOff(1, false);
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
	}
	// �z�B���A�����b�N�ς݂�������
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
	{
		// ����
		SetMenuOnOff(1, false);
	}
}
