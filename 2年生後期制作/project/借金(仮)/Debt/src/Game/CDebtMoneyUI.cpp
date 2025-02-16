#include "CDebtMoneyUI.h"
#include "CTextUI2D.h"
#include "CImage.h"
#include "CMoneyManager.h"
#include "CInput.h"

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

// �������̌����Ԋu
#define DECREASE_MONEY_INTERVAL 0.01f
// ��������l
#define DECREASE_AMOUNT 20

// �A���t�@�̌����Ԋu
#define DECREASE_ALPHA_INTERVAL 0.01f
// ��������A���t�@�l
#define DECREASE_ALPHA 0.2f
// ��������T�C�Y
#define DECREASE_SIZE 16

// �R���X�g���N�^
CDebtMoneyUI::CDebtMoneyUI()
	: CObjectBase(ETag::eUI, ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mElapsedTime(0.0f)
	, mDecreaseAmount(0)
	, mFontSize(DAY_FONT_SIZE)
{
	// �������̌�����Ԃ�
	ChangeState(EState::eDecrease);
	// �؋��z�̃e�L�X�g�𐶐�
	mpDay = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// �e�L�X�g��ݒ�
	mpDay->ChangeToStr("%d����\n", 0);
	// ������ݒ�
	mpDay->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// �I�t�Z�b�g���W��ݒ�
	mpDay->Position(DAY_OFFSET_POS);
	// �t�H���g�T�C�Y��ݒ�
	mpDay->SetFontSize(mFontSize);

	// �؋��z�̃e�L�X�g�𐶐�
	mpDebtMoney = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// �e�L�X�g��ݒ�
	mpDebtMoney->ChangeToStr("�����̕ԍϊz�F%d�~\n", 1000);
	// �ԍϊz��ۑ�
	mDebtMoneyAmount = 1000;
	// ������ݒ�
	mpDebtMoney->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// �I�t�Z�b�g���W��ݒ�
	mpDebtMoney->Position(DEBT_MONEY_OFFSET_POS);

	// ���������擾����
	CMoneyManager* moneyMgr = CMoneyManager::Instance();
	int money = moneyMgr->GetMoney();
	// �������̃e�L�X�g�𐶐�
	mpMoney = new CTextUI2D(ETaskPauseType::eMenu, false, nullptr);
	// �e�L�X�g��ݒ�
	mpMoney->ChangeToStr("�������F%d�~\n", money);
	// ��������ۑ�
	mMoneyAmount = money;
	// ������ݒ�
	mpMoney->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	// �I�t�Z�b�g���W��ݒ�
	mpMoney->Position(MONEY_OFFSET_POS);

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
	SAFE_DELETE(mpDebtMoney);
	SAFE_DELETE(mpMoney);
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
	}

	mpDay->Update();
	mpDebtMoney->Update();
	mpMoney->Update();
}

// �`��
void CDebtMoneyUI::Render()
{
	mpBackGround->Render();
	// �`�悷��Ȃ�
	if (mpDay->IsShow())
	{
		mpDay->Render();
	}
	if (mpDebtMoney->IsShow())
	{
		mpDebtMoney->Render();
	}
	if (mpMoney->IsShow())
	{
		mpMoney->Render();
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
	case 0:
		mElapsedTime += Times::DeltaTime();
		// �����Ԋu�̎��Ԃ��߂�����
		if (mElapsedTime >= DECREASE_MONEY_INTERVAL)
		{
			// ��������l�����炷
			mMoneyAmount -= DECREASE_AMOUNT;
			// �����������̂Ō��������l�𑝂₷
			mDecreaseAmount += DECREASE_AMOUNT;
			mElapsedTime -= DECREASE_MONEY_INTERVAL;
			// ���������l���ԍϊz�ɂȂ��
			if (mDebtMoneyAmount == mDecreaseAmount)
			{
				// ���̃X�e�b�v��
				mStateStep++;
			}
			// �������̃e�L�X�g��ݒ�
			mpMoney->ChangeToStr("�������F%d�~\n", mMoneyAmount);
		}
		break;

	case 1:
		// ���N���b�N��������
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �t�F�[�h�A�E�g��Ԃ�
			ChangeState(EState::eFadeOut);
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
			mpDay->SetFontSize(mFontSize);
			mpDebtMoney->SetFontSize(mFontSize / 2);
			mpMoney->SetFontSize(mFontSize / 2);
			// �A���t�@��0�ȉ��Ȃ�
			if (mpBackGround->GetAlpha() <= 0.0f)
			{
				// ��\��
				mpDay->SetShow(false);
				mpDebtMoney->SetShow(false);
				mpMoney->SetShow(false);
				// �ҋ@��Ԃ�
				ChangeState(EState::eIdle);
			}
		}
		break;
	}
}

// ��Ԑ؂�ւ�
void CDebtMoneyUI::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}
