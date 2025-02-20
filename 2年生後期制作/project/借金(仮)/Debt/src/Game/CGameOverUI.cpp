#include "CGameOverUI.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"
#include "CMoneyManager.h"
#include "CJobStatusManager.h"

// ���j���[�̃A�j���[�V��������
#define OPEN_ANIM_TIME 0.25f
// ���j���[�̃A�j���[�V������̑҂�����
#define OPENED_WAIT_TIME 0.5f

// �{�^���̃I�t�Z�b�g���W
#define BUTTON_OFFSET_POS1 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.5f)
#define BUTTON_OFFSET_POS2 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.75f)
// �{�^���̃T�C�Y
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// �w�i�摜�̃p�X
#define BG_PATH "UI/game_over_bg.png"

// �R���X�g���N�^
CGameOverUI::CGameOverUI()
	: CEndUIBase(BG_PATH)
{
	// �e�L�X�g��ݒ�
	mpText->SetStr("�Q�[���I�[�o�[");

	// [�O����]�{�^���𐶐�
	CExpandButton* btn1 = new CExpandButton
	(
		BUTTON_OFFSET_POS1,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn1->LoadButtonImage("UI/previous_day0.png", "UI/previous_day1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn1->SetOnClickFunc(std::bind(&CGameOverUI::OnClickPreDay, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn1->SetEnable(false);
	btn1->SetScale(0.0f);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn1);

	// [�I��]�{�^���𐶐�
	CExpandButton* btn2 = new CExpandButton
	(
		BUTTON_OFFSET_POS2,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn2->LoadButtonImage("UI/title_quit0.png", "UI/title_quit1.png");
	btn2->SetOnClickFunc(std::bind(&CGameOverUI::OnClickQuit, this));
	btn2->SetEnable(false);
	btn2->SetScale(0.0f);
	mButtons.push_back(btn2);
}

// �f�X�g���N�^
CGameOverUI::~CGameOverUI()
{
}

// �O���֖߂邩
bool CGameOverUI::IsPreDay() const
{
	// �I�����ڂ�1�ڂȂ�΁A�O����
	return mSelectIndex == 0;
}

// �Q�[�����I�����邩
bool CGameOverUI::IsExitGame() const
{
	// �I�����ڂ�2�ڂȂ�΁A�Q�[���I��
	return mSelectIndex == 1;
}

// �X�V
void CGameOverUI::Update()
{
	CEndUIBase::Update();
}

// �`��
void CGameOverUI::Render()
{
	CEndUIBase::Render();
}

// [�O����]�N���b�N���̃R�[���o�b�N�֐�
void CGameOverUI::OnClickPreDay()
{
	if (mIsEnd) return;

	// �����̊Ǘ��N���X���擾
	auto* moneyMgr = CMoneyManager::Instance();
	// ���[���o�b�N����
	moneyMgr->Rollback();

	// �d���̃X�e�[�^�X�Ǘ��N���X���擾
	auto* jobStatusMgr = CJobStatusManager::Instance();
	// ���[���o�b�N����
	jobStatusMgr->Rollback();

	mSelectIndex = 0;
	mIsEnd = true;
}

// [�I��]�N���b�N���̃R�[���o�b�N�֐�
void CGameOverUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}
