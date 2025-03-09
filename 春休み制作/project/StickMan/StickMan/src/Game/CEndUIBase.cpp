#include "CEndUIBase.h"
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

#define TEXT_OFFSET_POS CVector2(0.0f, 20.0f)

// �R���X�g���N�^
CEndUIBase::CEndUIBase(std::string bgPath)
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// �e�L�X�g�𐶐�
	mpText = new CTextUI2D(ETaskPauseType::eGame, false);
	mpText->SetFontSize(128);
	mpText->SetFontAligment(FTGL::TextAlignment::ALIGN_CENTER);
	mpText->Position(TEXT_OFFSET_POS);

	// �w�i�C���[�W�𐶐�
	mpBg = new CImage
	(
		bgPath.c_str(),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpBg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// �f�X�g���N�^
CEndUIBase::~CEndUIBase()
{
	SAFE_DELETE(mpText);
	SAFE_DELETE(mpBg);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// �Q�[���I�[�o�[��ʏI����
bool CEndUIBase::IsEnd() const
{
	return mIsEnd;
}

// �X�V
void CEndUIBase::Update()
{
	switch (mState)
	{
		// ���j���[���J��
	case EState::eOpen:
		UpdateOpen();
		break;
		// ���j���[�I��
	case EState::eSelect:
		UpdateSelect();
		break;
		// �t�F�[�h�A�E�g
	case EState::eFadeOut:
		UpdateFadeOut();
		break;
	}

	mpText->Update();
	mpBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// �`��
void CEndUIBase::Render()
{
	// �w�i�`��
	mpBg->Render();
	// �e�L�X�g�`��
	mpText->Render();

	// ���j���[�{�^����\��
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// ���j���[���J��
void CEndUIBase::UpdateOpen()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F���j���[�̓���A�j���[�V����
	case 0:
		if (mElapsedTime < OPEN_ANIM_TIME)
		{
			// �X�P�[���l����U1.0���傫�����āA1.0�֖߂�C�[�W���O�A�j���[�V����
			float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(scale);
			}
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(1.0f);
			}
			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
		// �X�e�b�v1�F���j���[�����̑҂�
	case 1:
		if (mElapsedTime < OPENED_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// ��莞�ԑ҂�����A�{�^�����I���ɂ��ă^�b�`�ł���悤�ɂ���
			// �i��^�b�`��h�����߂̑҂����ԁj
			for (CExpandButton* btn : mButtons)
			{
				btn->SetEnable(true);
			}
			ChangeState(EState::eSelect);
		}
		break;
	}
}

// ���j���[�I��
void CEndUIBase::UpdateSelect()
{
}

// �t�F�[�h�A�E�g
void CEndUIBase::UpdateFadeOut()
{
}

// ��Ԑ؂�ւ�
void CEndUIBase::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}