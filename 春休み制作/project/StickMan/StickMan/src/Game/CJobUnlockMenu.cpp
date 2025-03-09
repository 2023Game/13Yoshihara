#include "CJobUnlockMenu.h"
#include "CSceneManager.h"
#include "CJobStatusManager.h"
#include "CTextUI2D.h"
#include "CMoneyManager.h"

#define MENU_UNLOCK_TRASH "UI/menu_unlock_trash.png"
#define MENU_UNLOCK_DELIVERY "UI/menu_unlock_delivery.png"

#define MENU_CLOSE "UI/menu_close.png"

// ���ʉ��̉���
#define SE_VOLUME 0.5f

// �����e�L�X�g�̓��e
#define UNLOCK_TRUE_TEXT			"�i����ρj\n"
#define TRASH_UNLOCK_FALSE_TEXT		"�S�~�E�����������\n%d�~\n"
#define DELIVERY_UNLOCK_FALSE_TEXT	"�z�B���������\n%d�~\n"

// �R���X�g���N�^
CJobUnlockMenu::CJobUnlockMenu(CGameMenuBase* prevMenu)
	: CGameMenuBase(std::vector<std::string> {MENU_UNLOCK_TRASH, MENU_UNLOCK_DELIVERY, MENU_CLOSE})
{
	mpPreMenu = prevMenu;

	// �Ō�̗v�f�ȊO�̃N���b�N���̃R�[���o�b�N�֐���ݒ�
	mButtons[0]->SetOnClickFunc(std::bind(&CJobUnlockMenu::OnClickTrash, this));
	mButtons[1]->SetOnClickFunc(std::bind(&CJobUnlockMenu::OnClickDelivery, this));
	auto* jobStatusMgr = CJobStatusManager::Instance();
	int trashUnlockMoney = jobStatusMgr->GetUnlockMoney(EJobType::eTrash);
	int deliveryUnlockMoney = jobStatusMgr->GetUnlockMoney(EJobType::eDelivery);
	// �����e�L�X�g�̐ݒ�
	mMenuTexts[0]->ChangeToStr(TRASH_UNLOCK_FALSE_TEXT, trashUnlockMoney);
	mMenuTexts[1]->ChangeToStr(DELIVERY_UNLOCK_FALSE_TEXT, deliveryUnlockMoney);
}

// �f�X�g���N�^
CJobUnlockMenu::~CJobUnlockMenu()
{
}

// �X�V
void CJobUnlockMenu::Update()
{
	CGameMenuBase::Update();
	// �W���u�X�e�[�^�X�Ǘ��N���X
	auto* jobMgr = CJobStatusManager::Instance();
	// �S�~�E�����A�����b�N�ς݂�������
	if (jobMgr->GetUnlock(EJobType::eTrash))
	{
		// ����
		SetMenuOnOff(0, false);
		// �S�~�E��������ς݃e�L�X�g�ɕύX
		mMenuTexts[0]->SetStr(UNLOCK_TRUE_TEXT);
	}
	// �z�B���A�����b�N�ς݂�������
	if (jobMgr->GetUnlock(EJobType::eDelivery))
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
	// �L���������������点����A�����b�N
	if (mMenuOnOff[0] &&
		IncreaseMoney(EJobType::eTrash))
	{
		// �v�b�V����
		mpPushSE->Play(SE_VOLUME, true);
		// �d�����A�����b�N
		CJobStatusManager::Instance()->SetUnlock(EJobType::eTrash, true);
	}
	// ������������������Ȃ��Ȃ�u�U�[��
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// [DELIVERY]�N���b�N���̃R�[���o�b�N�֐�
void CJobUnlockMenu::OnClickDelivery()
{
	// �z�B
	// �L���������������点����A�����b�N
	if (mMenuOnOff[1] &&
		IncreaseMoney(EJobType::eDelivery))
	{
		// �v�b�V����
		mpPushSE->Play(SE_VOLUME, true);
		// �d�����A�����b�N
		CJobStatusManager::Instance()->SetUnlock(EJobType::eDelivery, true);
	}
	// ������������������Ȃ��Ȃ�u�U�[��
	else
	{
		mpBuzzerSE->Play(SE_VOLUME, true);
	}
}

// �A�����b�N���z�����点����
bool CJobUnlockMenu::IncreaseMoney(EJobType jobType)
{
	// �d�������邩
	bool isJob = false;
	// �����̊Ǘ��N���X���擾
	auto* moneyMgr = CMoneyManager::Instance();
	// �d���Ǘ��N���X���擾
	auto* jobStatusMgr = CJobStatusManager::Instance();
	// ������
	int money = moneyMgr->GetMoney();
	// �A�����b�N���z
	int unlockMoney = 0;

	// �S�~�E���̏ꍇ
	if (jobType == EJobType::eTrash)
	{
		// �A�����b�N���z��ݒ�
		unlockMoney = jobStatusMgr->GetUnlockMoney(EJobType::eTrash);
		// �d��������
		isJob = true;
	}
	// �z�B�̏ꍇ
	else if (jobType == EJobType::eDelivery)
	{
		// �A�����b�N���z���擾
		unlockMoney = jobStatusMgr->GetUnlockMoney(EJobType::eDelivery);
		// �d��������
		isJob = true;
	}
	// �d�����Ȃ��Ȃ玸�s
	if (!isJob) return false;

	// �A�����b�N���z�����Z
	money -= unlockMoney;
	// ��������0��菬�����Ȃ�A�����b�N���s
	if (money < 0) return false;

	// ��������ݒ�
	moneyMgr->SetMoney(money);
	// �A�����b�N����
	return true;
}
