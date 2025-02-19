#include "CGameOverUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// ���j���[�̃A�j���[�V��������
#define OPEN_ANIM_TIME 0.25f
// ���j���[�̃A�j���[�V������̑҂�����
#define OPENED_WAIT_TIME 0.5f

// �{�^���̃I�t�Z�b�g���W
#define BUTTON_OFFSET_POS1 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.5f)
#define BUTTON_OFFSET_POS2 CVector2(WINDOW_WIDTH * 0.825f, WINDOW_HEIGHT * 0.75f)
// �{�^���̃T�C�Y
#define BUTTON_SIZE CVector2(340.0f, 96.0f)

// �R���X�g���N�^
CGameOverUI::CGameOverUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eOpen)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// �t�H���g�f�[�^�𐶐�
	mpFont = new CFont("res\\Font\\toroman.ttf");
	mpFont->SetFontSize(128);
	mpFont->SetAlignment(FTGL::TextAlignment::ALIGN_CENTER);
	mpFont->SetLineLength(WINDOW_WIDTH);

	// �Q�[���I�[�o�[�̃e�L�X�g�𐶐�
	mpGameOverText = new CText
	(
		mpFont, 128,
		CVector2(0.0f, 20.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(0.1f,0.1f,0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpGameOverText->SetText("�Q�[���I�[�o�[");
	mpGameOverText->SetEnableOutline(true);
	mpGameOverText->SetOutlineColor(CColor(0.9f, 0.9f, 0.9f));

	// �Q�[���I�[�o�[��ʂ̔w�i�C���[�W�𐶐�
	mpGameOverBg = new CImage
	(
		"UI/game_over_bg.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpGameOverBg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

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
	SAFE_DELETE(mpFont);
	SAFE_DELETE(mpGameOverText);
	SAFE_DELETE(mpGameOverBg);

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
bool CGameOverUI::IsEnd() const
{
	return mIsEnd;
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

	mpGameOverText->Update();
	mpGameOverBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// �`��
void CGameOverUI::Render()
{
	// �w�i�`��
	mpGameOverBg->Render();
	// �Q�[���I�[�o�[�e�L�X�g�`��
	mpGameOverText->Render();

	// ���j���[�{�^����\��
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// ���j���[���J��
void CGameOverUI::UpdateOpen()
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
void CGameOverUI::UpdateSelect()
{
}

// �t�F�[�h�A�E�g
void CGameOverUI::UpdateFadeOut()
{
}

// ��Ԑ؂�ւ�
void CGameOverUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [�O����]�N���b�N���̃R�[���o�b�N�֐�
void CGameOverUI::OnClickPreDay()
{
	if (mIsEnd) return;

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
