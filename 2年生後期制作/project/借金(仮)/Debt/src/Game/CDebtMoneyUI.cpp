#include "CDebtMoneyUI.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CMoneyManager.h"
#include "CJobStatusManager.h"
#include "CInput.h"
#include "CBGMManager.h"
#include "CTaskManager.h"
#include "CSceneManager.h"

// �w�i�摜�̃p�X
#define BG_PATH "UI/white.png"

// �����̃I�t�Z�b�g���W
#define DAY_OFFSET_POS CVector2(0.0f, WINDOW_HEIGHT * 0.1f)
// �����̃t�H���g�T�C�Y
#define DAY_FONT_SIZE 128

// �؋��z�̃I�t�Z�b�g���W
#define DEBT_MONEY_OFFSET_POS CVector2(0.0f, WINDOW_HEIGHT * 0.4f)

// �������̃I�t�Z�b�g���W
#define MONEY_OFFSET_POS CVector2(0.0f, WINDOW_HEIGHT * 0.8f)

// ���ւ̃I�t�Z�b�g���W
#define NEXT_OFFSET_POS CVector2(0.0f, WINDOW_HEIGHT * 0.9f)
// ���ւ̃t�H���g�T�C�Y
#define NEXT_FONT_SIZE 32
// ���ւ̃e�L�X�g
#define NEXT_TEXT1 "�N���b�N�ŕԍ�"
#define NEXT_TEXT2 "�N���b�N�ŃX�L�b�v"
#define NEXT_TEXT3 "�N���b�N�ŕ���"

// �������̌����Ԋu
#define DECREASE_MONEY_INTERVAL 0.01f
// ��������l
#define DECREASE_AMOUNT1000 1000	// 1000�̈�
#define DECREASE_AMOUNT100   100	// 100�̈�
#define DECREASE_AMOUNT10     10	// 10�̈�
#define DECREASE_AMOUNT1	   1	// 1�̈�

// �A���t�@�̌����Ԋu
#define DECREASE_ALPHA_INTERVAL 0.01f
// ��������A���t�@�l
#define DECREASE_ALPHA 0.1f
// ��������T�C�Y
#define DECREASE_SIZE 16

// �R���X�g���N�^
CDebtMoneyUI::CDebtMoneyUI()
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 3, ETaskPauseType::eMenu)
	, mElapsedTime(0.0f)
	, mResultAmount(0)
	, mFontSize(DAY_FONT_SIZE)
	, mpDayText(nullptr)
	, mpDebtMoneyText(nullptr)
	, mpMoneyText(nullptr)
	, mpBackGround(nullptr)
{
	// �����̊Ǘ��N���X���擾����
	CMoneyManager* moneyMgr = CMoneyManager::Instance();
	// ���ɕԍς��Ă���
	if (moneyMgr->GetDid())
	{
		// �����ɂ���
		SetEnable(false);
		SetShow(false);
		// ����ȉ����������Ȃ�
		return;
	}
	// �J�[�\�����o��
	CInput::ShowCursor(true);
	// BGM��ݒ�
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	// �|�[�Y
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
	// �������̌�����Ԃ�
	ChangeState(EState::eDecrease);
	// ������
	mMoneyAmount = moneyMgr->GetMoney();
	// �ԍϊz
	mDebtMoneyAmount = moneyMgr->GetDebtMoney();
	// ����
	int day = moneyMgr->GetDay();
	// ����
	mResultAmount = mMoneyAmount - mDebtMoneyAmount;

	// �����̃e�L�X�g�𐶐�
	mpDayText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// �e�L�X�g��ݒ�
	mpDayText->ChangeToStr("%d����\n", day);
	// ������ݒ�
	mpDayText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// �I�t�Z�b�g���W��ݒ�
	mpDayText->Position(DAY_OFFSET_POS);
	// �t�H���g�T�C�Y��ݒ�
	mpDayText->SetFontSize(mFontSize);

	// �؋��z�̃e�L�X�g�𐶐�
	mpDebtMoneyText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// �e�L�X�g��ݒ�
	mpDebtMoneyText->ChangeToStr("�����̕ԍϊz�F%d�~\n", mDebtMoneyAmount);
	// ������ݒ�
	mpDebtMoneyText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// �I�t�Z�b�g���W��ݒ�
	mpDebtMoneyText->Position(DEBT_MONEY_OFFSET_POS);

	// �������̃e�L�X�g�𐶐�
	mpMoneyText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// �e�L�X�g��ݒ�
	mpMoneyText->ChangeToStr("�������F%d�~\n", mMoneyAmount);
	// ������ݒ�
	mpMoneyText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// �I�t�Z�b�g���W��ݒ�
	mpMoneyText->Position(MONEY_OFFSET_POS);

	// �������̃e�L�X�g�𐶐�
	mpNextText = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// �e�L�X�g��ݒ�
	mpNextText->SetStr(NEXT_TEXT1);
	// ������ݒ�
	mpNextText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// �I�t�Z�b�g���W��ݒ�
	mpNextText->Position(NEXT_OFFSET_POS);
	// �t�H���g�T�C�Y��ݒ�
	mpNextText->SetFontSize(NEXT_FONT_SIZE);
	// �t�H���g�J���[��ݒ�
	mpNextText->SetFontColor(CColor(1.0f, 1.0f, 0.5f));
	// �e��ݒ�
	mpNextText->SetShadow(true, CColor(0.25f, 0.125f, 0.0f));
	// �A�E�g���C���J���[��ݒ�
	mpNextText->SetOutline(true, CColor(0.0f, 0.0f, 0.0f));
	// �_�ł���
	mpNextText->SetBlink(true);

	mpBackGround = new CImage
	(
		BG_PATH,
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpBackGround->SetColor(CColor::black);
	mpBackGround->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// �f�X�g���N�^
CDebtMoneyUI::~CDebtMoneyUI()
{
	SAFE_DELETE(mpDayText);
	SAFE_DELETE(mpDebtMoneyText);
	SAFE_DELETE(mpMoneyText);
	SAFE_DELETE(mpBackGround);
}

// �X�V
void CDebtMoneyUI::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eDecrease:
		UpdateDecrease();
		break;
	case EState::eFadeOut:
		UpdateFadeOut();
		break;
	case EState::eGameOver:
		UpdateGameOver();
		break;
	}

	mpDayText->Update();
	mpDebtMoneyText->Update();
	mpMoneyText->Update();
	mpNextText->Update();
}

// �`��
void CDebtMoneyUI::Render()
{
	mpBackGround->Render();
	mpDayText->Render();
	mpDebtMoneyText->Render();
	mpMoneyText->Render();
	// ������ԂȂ�`��
	if (mState == EState::eDecrease)
	{
		mpNextText->Render();
	}
}

// �ҋ@
void CDebtMoneyUI::UpdateIdle()
{
}

// �������̌���
void CDebtMoneyUI::UpdateDecrease()
{
	switch (mStateStep)
	{
		// ���N���b�N�Ŏ���
	case 0:
		// ���N���b�N��������
		if (CInput::PushKey(VK_LBUTTON))
		{
			// ���ւ̃e�L�X�g��ݒ�
			mpNextText->SetStr(NEXT_TEXT2);
			// ���̃X�e�b�v��
			mStateStep++;
		}
		break;
		// �����������炷
		// ���N���b�N�Ŏ��̏�Ԃ�
	case 1:
		mElapsedTime += Times::DeltaTime();
		// �����Ԋu�̎��Ԃ��߂�����
		if (mElapsedTime >= DECREASE_MONEY_INTERVAL)
		{
			// �����������炷
			DecreaseMoney();
			mElapsedTime -= DECREASE_MONEY_INTERVAL;
			// �ԍϊz��0�ȉ��Ȃ�
			// �������́A���N���b�N�ŃX�L�b�v
			if (mDebtMoneyAmount <= 0 ||
				CInput::PushKey(VK_LBUTTON))
			{
				// �ԍϊz��0�ɂ���
				mDebtMoneyAmount = 0;
				// �����������ʂɂ���
				mMoneyAmount = mResultAmount;
				// ��������0�ȏ�Ȃ�
				if (mMoneyAmount >= 0)
				{
					// �����̊Ǘ��N���X���擾
					auto* moneyMgr = CMoneyManager::Instance();
					// �O�̓��̏�������ݒ�
					moneyMgr->SetPreMoney(moneyMgr->GetMoney());

					// �d���̃X�e�[�^�X�Ǘ��N���X���擾
					auto* jobStatusMgr = CJobStatusManager::Instance();
					// �O�̓��̃X�e�[�^�X��ݒ�
					jobStatusMgr->SetPreJobStatus();

					// �ԍς���
					moneyMgr->SetDid(true);
					// �������Ɍ��ʂ�ݒ�
					moneyMgr->SetMoney(mMoneyAmount);
				}
				// ���ւ̃e�L�X�g��ݒ�
				mpNextText->SetStr(NEXT_TEXT3);
				// ����
				mStateStep++;
			}

			// �������̃e�L�X�g��ݒ�
			mpMoneyText->ChangeToStr("�������F%d�~\n", mMoneyAmount);
			// �ԍϊz�̃e�L�X�g��ݒ�
			mpDebtMoneyText->ChangeToStr("�ԍϊz�F%d�~\n", mDebtMoneyAmount);
		}
		break;
	case 2:
		// ���N���b�N��������
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �|�[�Y����
			CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
			// ���ʂ�0�ȏ�Ȃ�
			if (mResultAmount >= 0)
			{
				// �����̊Ǘ��N���X���擾
				auto* moneyMgr = CMoneyManager::Instance();
				// �N���A�̓����Ȃ�
				if (moneyMgr->IsClear())
				{
					// �V�[���Ǘ��N���X���擾
					auto* sceneMgr = CSceneManager::Instance();
					// �Q�[���N���A�V�[����
					sceneMgr->LoadScene(EScene::eGameClear);
				}
				else
				{
					// �t�F�[�h�A�E�g��Ԃ�
					ChangeState(EState::eFadeOut);
					// BGM��ݒ�
					CBGMManager::Instance()->Play(EBGMType::eHome, false);
				}
			}
			// ��������0��菬�����Ȃ�
			else
			{
				// �Q�[���I�[�o�[��
				ChangeState(EState::eGameOver);
			}
		}
		break;
	}
}

// �t�F�[�h�A�E�g
void CDebtMoneyUI::UpdateFadeOut()
{
	mElapsedTime += Times::DeltaTime();
	switch (mStateStep)
	{
		// �t�F�[�h�A�E�g����
	case 0:
		// �����Ԋu�̎��Ԃ��߂�����
		if (mElapsedTime >= DECREASE_ALPHA_INTERVAL)
		{
			mElapsedTime -= DECREASE_ALPHA_INTERVAL;
			// �t�H���g�T�C�Y������
			mFontSize -= DECREASE_SIZE;
			// �A���t�@������
			mpBackGround->SetAlpha(mpBackGround->GetAlpha() - DECREASE_ALPHA);
			mpDayText->SetFontSize(mFontSize);
			mpDebtMoneyText->SetFontSize(mFontSize / 2);
			mpMoneyText->SetFontSize(mFontSize / 2);
			// �A���t�@��0�ȉ��Ȃ�
			if (mpBackGround->GetAlpha() <= 0.0f)
			{
				// �J�[�\���𖳌�
				CInput::ShowCursor(false);
				// ��\��
				SetEnable(false);
				SetShow(false);
				// �ҋ@��Ԃ�
				ChangeState(EState::eIdle);
			}
		}
		break;
	}
}

// �Q�[���I�[�o�[
void CDebtMoneyUI::UpdateGameOver()
{
	// �Q�[���I�[�o�[�V�[����
	CSceneManager::Instance()->LoadScene(EScene::eGameOver);
}

// ��Ԑ؂�ւ�
void CDebtMoneyUI::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// �����������炷
void CDebtMoneyUI::DecreaseMoney()
{			
	// 10000�ȏ゠��Ȃ�
	if (mDebtMoneyAmount >= DECREASE_AMOUNT1000 * 10)
	{
		// 1000���炷
		mMoneyAmount -= DECREASE_AMOUNT1000;
		mDebtMoneyAmount -= DECREASE_AMOUNT1000;
	}
	// 1000�ȏ゠��Ȃ�
	if (mDebtMoneyAmount >= DECREASE_AMOUNT100 * 10)
	{
		// 100���炷
		mMoneyAmount -= DECREASE_AMOUNT100;
		mDebtMoneyAmount -= DECREASE_AMOUNT100;
	}
	// 100�ȏ゠��Ȃ�
	if (mDebtMoneyAmount >= DECREASE_AMOUNT10 * 10)
	{
		// 10���炷
		mMoneyAmount -= DECREASE_AMOUNT10;
		mDebtMoneyAmount -= DECREASE_AMOUNT10;
	}
	// 1���炷
	mMoneyAmount -= DECREASE_AMOUNT1;
	mDebtMoneyAmount -= DECREASE_AMOUNT1;
}
