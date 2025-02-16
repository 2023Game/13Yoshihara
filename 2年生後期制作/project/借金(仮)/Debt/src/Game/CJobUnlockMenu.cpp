#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"
#include "CTextUI2D.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"

// ���ʉ��̉���
#define SE_VOLUME 0.5f

// �����e�L�X�g�̓��e
#define UNLOCK_TRUE_TEXT		"�i����ρj\n"
#define TRASH_UNLOCK_FALSE_TEXT		"�S�~�E�����������\n2000�~\n"
#define DELIVERY_UNLOCK_FALSE_TEXT	"�z�B���������\n2000�~\n"

// �R���X�g���N�^
CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE})
{
	mpPrevMenu = prevMenu;

	// �Ō�̗v�f�ȊO�̃N���b�N���̃R�[���o�b�N�֐���ݒ�
	mButtons[0]->SetOnClickFunc(std::bind(&CJobUnlockMenu::OnClickTrash, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CJobUnlockMenu::OnClickDelivery, this));
	// �����e�L�X�g�̐ݒ�
	mMenuTexts[0]->SetStr(TRASH_UNLOCK_FALSE_TEXT);
	mMenuTexts[1]->SetStr(DELIVERY_UNLOCK_FALSE_TEXT);
}

// �f�X�g���N�^
CJobUnlockMenu::~CJobUnlockMenu()
{
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
		mMenuTexts[0]->SetStr(UNLOCK_TRUE_TEXT);
	}
	// �z�B���A�����b�N�ς݂�������
	if (CJobStatusManager::Instance()->GetUnlock(EJobType::eDelivery))
	{
		// ����
		SetMenuOnOff(1, false);
		// �z�B������ς݃e�L�X�g�ɕύX
		mMenuTexts[1]->SetStr(UNLOCK_TRUE_TEXT);
	}
}

// [TRASH]�N���b�N���̃R�[���o�b�N�֐�
void CJobUnlockMenu::OnClickTrash()
{
	// �S�~�E��
	// �L���Ȃ�A�����b�N
	if (mMenuOnOff[0])
	{
		// �v�b�V����
		mpPushSE->Play(SE_VOLUME, true);
		CJobStatusManager::Instance()->SetUnlock(EJobType::eTrash, true);
	}
	// �����Ȃ�u�U�[��
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// [DELIVERY]�N���b�N���̃R�[���o�b�N�֐�
void CJobUnlockMenu::OnClickDelivery()
{
	// �z�B
	// �L���Ȃ�A�����b�N
	if (mMenuOnOff[1])
	{
		// �v�b�V����
		mpPushSE->Play(SE_VOLUME, true);
		CJobStatusManager::Instance()->SetUnlock(EJobType::eDelivery, true);
	}
	// �����Ȃ�u�U�[��
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}
